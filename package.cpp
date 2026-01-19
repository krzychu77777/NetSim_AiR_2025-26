#include "package.hxx"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

Package::Package(ElementID id) : id_(id) {
    assigned_IDs.insert(id_);
    freed_IDs.erase(id_);
}

Package::Package(Package&& other) noexcept {
    this->id_ = other.id_;
    other.id_ = 0;
}

Package& Package::operator=(Package&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    release_id(this->id_);
    this->id_ = other.id_;
    other.id_ = 0;

    return *this;
}

ElementID Package::acquire_id() {
    ElementID new_id;

    if (!freed_IDs.empty()) {
        auto it = freed_IDs.begin();
        new_id = *it;
        freed_IDs.erase(it);
    }
    else {
        if (assigned_IDs.empty()) {
            new_id = 1;
        }
        else {
            new_id = *assigned_IDs.rbegin() + 1;
        }
    }
    assigned_IDs.insert(new_id);

    return new_id;
}

void Package::release_id(ElementID id) {
    assigned_IDs.erase(id);
    freed_IDs.insert(id);
}

Package::~Package() {
    if (id_ > 0) {
        release_id(id_);
    }
}