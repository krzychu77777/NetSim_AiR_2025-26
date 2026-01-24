#include "nodes.hxx"

void Ramp::deliver_goods(Time t) {

}

void PackageSender::send_package() {
    // do zaimplementowania kiedy pojawi się przekazywanie paczek do sąsiadów
}

void Worker::do_work(Time t) {
    if (!current_package.has_value()) {
        if (!queue_->empty()){
            current_package = queue_->pop();
            processing_start = t;
        }
    } else {
        if (t - processing_start >= processing_duration_) {
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