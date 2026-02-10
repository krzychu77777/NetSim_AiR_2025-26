#include "factory.hxx"
#include <sstream>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) { return true; }
    if (node_colors[sender] == NodeColor::VISITED) { return false; }

    node_colors[sender] = NodeColor::VISITED;

    std::map<IPackageReceiver*, double> receivers = sender->receiver_preferences_.get_preferences();

    for (const auto& pair : receivers) {
        IPackageReceiver* receiver = pair.first;

        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE) {
            node_colors[sender] = NodeColor::VERIFIED;
            return true;
        }

        if (receiver->get_receiver_type() == ReceiverType::WORKER) {
            IPackageReceiver* ptr = receiver;
            auto worker_ptr = dynamic_cast<Worker*>(ptr);
            auto sendrecv_ptr = dynamic_cast<const PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) continue;

            bool found = has_reachable_storehouse(sendrecv_ptr, node_colors);

            if (found) {
                node_colors[sender] = NodeColor::VERIFIED;
                return true;
            }
        }
    }
    return false;
}


bool Factory::is_consistent() {
    // inicjalizacja mapy kolorów węzłów
    std::map<const PackageSender*, NodeColor> node_color;
    for (auto it = ramp_.cbegin(); it != ramp_.cend(); ++it) {
        node_color[&(*it)] = NodeColor::UNVISITED;
    }
    for (auto it = worker_.cbegin(); it != worker_.cend(); ++it) {
        node_color[&(*it)] = NodeColor::UNVISITED;
    }
    for (auto it = ramp_.cbegin(); it != ramp_.cend(); ++it) {
        const PackageSender* sender_ptr = &(*it);
        if (!has_reachable_storehouse(sender_ptr, node_color)) {
            return false;
        }
    }
    for (auto it = worker_.cbegin(); it != worker_.cend(); ++it) {
        const PackageSender* sender_ptr = &(*it);
        if (!has_reachable_storehouse(sender_ptr, node_color)) {
            return false;
        }
    }
    return true;
}

void Factory::do_deliveries(Time t) {
    for (auto it = ramp_.cbegin(); it != ramp_.cend(); ++it) {
        it->deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto it = ramp_.cbegin(); it != ramp_.cend(); ++it) {
        it->send_package();
    }
    for (auto it = worker_.cbegin(); it != worker_.cend(); ++it) {
        it->send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto it = worker_.cbegin(); it != worker_.cend(); ++it) {
        it->do_work(t);
    }
}

void Factory::remove_receiver_links(IPackageReceiver* receiver) {
    for (auto it = ramp_.begin(); it != ramp_.end(); ++it) {
        it->receiver_preferences_.remove_receiver(receiver);
    }
    
    for (auto it = worker_.begin(); it != worker_.end(); ++it) {
        it->receiver_preferences_.remove_receiver(receiver);
    }
}

void Factory::remove_worker(ElementID id) {
    auto it = worker_.find_by_id(id);
    if (it != worker_.end()) {
        remove_receiver_links(&(*it));
        worker_.remove_by_id(id);
    }
}

void Factory::remove_storehouse(ElementID id) {
    auto it = storehouse_.find_by_id(id);
    if (it != storehouse_.end()) {
        remove_receiver_links(&(*it));
        storehouse_.remove_by_id(id);
    }
}

std::string parse_value(const std::string& token) {
    auto delimiter_pos = token.find('=');
    if (delimiter_pos != std::string::npos) {
        return token.substr(delimiter_pos + 1);
    }
    return "";
}

Factory load_factory_structure(std::istream& is) {
    Factory factory;
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "LOADING_RAMP") {
            ElementID id = -1;
            TimeOffset di = 1;
            std::string token;

            while (ss >> token) {
                if (token.find("id=") == 0) {
                    id = std::stoi(parse_value(token));
                }
                else if (token.find("delivery-interval=") == 0) {
                    di = std::stoi(parse_value(token));
                }
            }
                factory.add_ramp(Ramp(id, di));
        }
        else if (type == "WORKER") {
            ElementID id = -1;
            TimeOffset pd = 1;
            PackageQueueType qt = PackageQueueType::FIFO;
            std::string token;

            while (ss >> token) {
                if (token.find("id=") == 0) {
                    id = std::stoi(parse_value(token));
                }
                else if (token.find("processing-time=") == 0) {
                    pd = std::stoi(parse_value(token));
                }
                else if (token.find("queue-type=") == 0) {
                    std::string val = parse_value(token);
                    if (val == "LIFO") {
                        qt = PackageQueueType::LIFO;
                    }
                    else {
                        qt = PackageQueueType::FIFO;
                    }
                }
            }
            factory.add_worker(Worker(id, pd, std::make_unique<PackageQueue>(qt)));
        }
        else if (type == "STOREHOUSE") {
            ElementID id = -1;
            std::string token;

            while (ss >> token) {
                if (token.find("id") == 0) {
                    id = std::stoi(parse_value(token));
                }
            }
            factory.add_storehouse(Storehouse(id));
        }
        else if (type == "LINK") {
            std::string token;
            std::string src_str, dest_str;

            while (ss >> token) {
                if (token.find("src=") == 0) {
                    src_str = parse_value(token);
                }
                else if (token.find("dest=") == 0) {
                    dest_str = parse_value(token);
                }
            }
            PackageSender* sender = nullptr;
            auto dash_pos = src_str.find('-');
            if (dash_pos != std::string::npos) {
                std::string src_type = src_str.substr(0, dash_pos);
                ElementID src_id = std::stoi(src_str.substr(dash_pos + 1));

                if (src_type == "ramp") {
                    auto it = factory.find_ramp_by_id(src_id);
                    if (it != factory.ramp_cend()) {
                        sender = &(*it);
                    }
                }
                else if (src_type == "worker") {
                    auto it = factory.find_worker_by_id(src_id);
                    if (it != factory.worker_cend()) {
                        sender = &(*it);
                    }
                }
            }

            IPackageReceiver* receiver = nullptr;
            dash_pos = dest_str.find('-');
            if (dash_pos != std::string::npos) {
                std::string dest_type = dest_str.substr(0, dash_pos);
                ElementID dest_id = std::stoi(dest_str.substr(dash_pos + 1));

                if (dest_type == "worker") {
                    auto it = factory.find_worker_by_id(dest_id);
                    if (it != factory.worker_cend()) {
                        receiver = &(*it);
                    }
                }
                else if (dest_type == "storehouse" || dest_type == "store") {
                   auto it = factory.find_storehouse_by_id(dest_id);
                    if (it != factory.storehouse_cend()) {
                        receiver = &(*it);
                    }
                }
                if (sender && receiver) {
                    sender->receiver_preferences_.add_receiver(receiver);
                }
            }
            
        }
    }
    return factory;
}

void save_factory_structure(const Factory& factory, std::ostream& os) {
    auto ramp_begin = factory.ramp_cbegin();
    auto ramp_end = factory.ramp_cend();
    for (auto it = ramp_begin; it != ramp_end; ++it) {
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << "\n";
    }

    auto worker_begin = factory.worker_cbegin();
    auto worker_end = factory.worker_cend();
    for (auto it = worker_begin; it != worker_end; ++it) {
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration() << " queue-type=";
        if (it->get_queue()->get_queue_type() == PackageQueueType::LIFO) {
            os << "LIFO";
        }
        else {
            os << "FIFO";
        }
        os << "\n";
    }

    auto store_begin = factory.storehouse_cbegin();
    auto store_end = factory.storehouse_cend();
    for (auto it = store_begin; it != store_end; ++it) {
        os << "STOREHOUSE id=" << it->get_id() << "\n";
    }

    auto print_links = [&](const PackageSender& sender, std::string src_type, ElementID src_id) {
        const auto& prefs = sender.receiver_preferences_.get_preferences();
        
        std::vector<IPackageReceiver*> sorted_receivers;
        for (const auto& pair : prefs) {
            sorted_receivers.push_back(pair.first);
        }
        std::sort(sorted_receivers.begin(), sorted_receivers.end(), [](IPackageReceiver* a, IPackageReceiver* b) {
            return a->get_id() < b->get_id();
        });
        for (auto receiver : sorted_receivers) {
            os << "LINK src=" << src_type << "-" << src_id << " dest=";
            if (receiver->get_receiver_type() == ReceiverType::WORKER) {
                os << "worker-" << receiver->get_id();
            }
            else {
                os << "store-" << receiver->get_id();
            }
            os << "\n";
        }
    };

    for (auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); ++it) {
        print_links(*it, "ramp", it->get_id());
    }

    for (auto it = factory.worker_cbegin(); it != factory.worker_cend(); ++it) {
        print_links(*it, "worker", it->get_id());
    }
}