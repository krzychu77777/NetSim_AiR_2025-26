#ifndef FACTORY.HXX
#define FACTORY.HXX

#include "nodes.hxx"

class Factory {
    public:
        Factory() = default;
        ~Factory() = default;

        //========== RAMPS ================
        void add_ramp(Ramp&& ramp) {};
        void remove_ramp(ElementID id) {};
        NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {};
        NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) {};
        NodeCollection<Ramp>::const_iterator ramps_cbegin() const {};
        NodeCollection<Ramp>::const_iterator ramps_cend() const {};

        //========== WORKER ================
        void add_worker(Worker&& worker) {};
        void remove_worker(ElementID id) {};
        NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {};
        NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) {};
        NodeCollection<Worker>::const_iterator workers_cbegin() const {};
        NodeCollection<Worker>::const_iterator workers_cend() const {};

        //======== STOREHOUSE ==============
        void add_storehouse(Storehouse&& storehouse) {};
        void remove_storehouse(ElementID id) {};
        NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {};
        NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) {};
        NodeCollection<Storehouse>::const_iterator storehouses_cbegin() const {};
        NodeCollection<Storehouse>::const_iterator storehouses_cend() const {};

    private:
        void remove_receiver(NodeCollection<Node>& collection, ElementID id) {};

        bool is_consistent() {};
        void do_deliveries(Time t) {};
        void do_package_passing() {};
        void do_work(Time t) {};
};

class NodeCollection {

};

#endif