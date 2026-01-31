#include "nodes.hxx"

// jeszcze do przemyślenia:
void Ramp::deliver_goods(Time t) {
    if ((t - 1) % di_ == 0) {
        Package p;
        push_package(std::move(p));
    }
}

void PackageSender::send_package() {
    if (buffer_) {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        if (receiver != nullptr) {
            receiver->receive_package(std::move(*buffer_));
            buffer_.reset();
        }
    }
}

void Worker::do_work(Time t) {
    if (!current_package.has_value()) {
        if (!queue_->empty()){
            current_package = queue_->pop();
            processing_start = t;
        }
    } else {
        if (t - processing_start +1 >= processing_duration_) {
            push_package(std::move(current_package.value()));
            current_package = std::nullopt;
        }
    }
}

Time Worker::get_package_processing_start() const {
    if (current_package.has_value()) {
        return processing_start;
    } else {
        return Time{};
    }
}

void ReceiverPreferences::rebuild_probabilities() {
    if (preferences_.empty()) {
        return;
    }

    double new_prob = 1.0 / preferences_.size();
    for (auto& item : preferences_) {
        item.second = new_prob;
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    preferences_[r] = 1.0;
    rebuild_probabilities();
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    size_t erased_count = preferences_.erase(r);
    if (erased_count > 0) {
        rebuild_probabilities();
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    if (preferences_.empty()) {
        return nullptr;
    }

    double p = pg_();
    double distribution = 0.0;

    for (const auto& item : preferences_) {
        IPackageReceiver* receiver = item.first;
        double probability = item.second;
        distribution += probability;

        if (p <= distribution) {
            return receiver;
        }
    }
    // zabezpieczenie przed błędami zaokrągleń, zwracam ostatniego dostępnego odbiorcę:
    return preferences_.rbegin()->first;
}