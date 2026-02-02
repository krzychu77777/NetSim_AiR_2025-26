#include "factory.hxx"

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {
    if (node_colors[sender] == NodeColor::VERIFIED) {return true;}

    node_colors[sender] = NodeColor::VISITED;

    bool does_bro_have_friends = false;
    std::map<IPackageReceiver*, double> receivers = sender->receiver_preferences_.get_preferences();
    for(const auto& [receiver, trash]: receivers) {
        if((receiver->get_receiver_type()) == ReceiverType::STOREHOUSE) {
            does_bro_have_friends = true;
            continue;
        }
        else {
            IPackageReceiver* receiver_ptr = receiver;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);

            if (sendrecv_ptr == sender) {
                continue;
            }
            does_bro_have_friends = true;

            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }
        node_colors[sender] = NodeColor::VERIFIED;
        if (does_bro_have_friends) {
            return true;
        }
        else {
            throw std::logic_error("No reachable storehouse found");
        }
    }
}


bool Factory::is_consistent() {
    // inicjalizacja mapy kolorów węzłów
    using color = std::map<const PackageSender*, NodeColor>;
    color node_color;

    // sprawdzamy tylko nadawców (Rampy i Workerów), póki co są nieodwiedzeni
    for (auto ramp_ptr = ramps_.cbegin(); ramp_ptr != ramps_.cend(); ++ramp_ptr) {
        node_color[ ramp_ptr->get() ] = NodeColor::UNVISITED;
    }
    for (auto worker_ptr = workers_.cbegin(); worker_ptr != workers_.cend(); ++worker_ptr) {
        node_color[ worker_ptr->get() ] = NodeColor::UNVISITED;
    }

    // DFSik czy cuś
    for (auto ramp_ptr = ramps_.cbegin(); ramp_ptr != ramps_.cend(); ++ramp_ptr) {
        try{
            has_reachable_storehouse(ramp_ptr->get(), node_color);
        } catch (std::logic_error& e) {
            return false;
        }
    }
    return true;
}

void Factory::do_deliveries(Time t) {
    for (auto ramp_ptr = ramps_.cbegin(); ramp_ptr != ramps_.cend(); ++ramp_ptr) {
        ramp_ptr->get()->deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto ramp_ptr = ramps_.cbegin(); ramp_ptr != ramps_.cend(); ++ramp_ptr) {
        ramp_ptr->get()->send_package();
    }
    for (auto worker_ptr = workers_.cbegin(); worker_ptr != workers_.cend(); ++worker_ptr) {
        worker_ptr->get()->send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto worker_ptr = workers_.cbegin(); worker_ptr != workers_.cend(); ++worker_ptr) {
        worker_ptr->get()->do_work(t);
    }
}
