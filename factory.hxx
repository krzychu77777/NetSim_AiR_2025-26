#ifndef FACTORY.HXX
#define FACTORY.HXX

#include "nodes.hxx"

class Factory {
    public:
        Factory() = default;
        ~Factory() = default;

        //========== RAMPS ================
        void add_ramp(Ramp&& ramp) { ramps_.add(std::move(ramp)); }
        void remove_ramp(ElementID id) { ramps_.remove_by_id(id); }
        NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
        NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) { return ramps_.find_by_id(id); }
        NodeCollection<Ramp>::const_iterator ramps_cbegin() const { return ramps_.cbegin(); }
        NodeCollection<Ramp>::const_iterator ramps_cend() const { return ramps_.cend(); }

        //========== WORKER ================
        void add_worker(Worker&& worker) { workers_.add(std::move(worker)); }
        void remove_worker(ElementID id) { workers_.remove_by_id(id); }
        NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
        NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) { return workers_.find_by_id(id); }
        NodeCollection<Worker>::const_iterator workers_cbegin() const { return workers_.cbegin(); }
        NodeCollection<Worker>::const_iterator workers_cend() const { return workers_.cend(); }

        //======== STOREHOUSE ==============
        void add_storehouse(Storehouse&& storehouse) { storehouses_.add(std::move(storehouse)); }
        void remove_storehouse(ElementID id) { storehouses_.remove_by_id(id); }
        NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }
        NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) { return storehouses_.find_by_id(id); }
        NodeCollection<Storehouse>::const_iterator storehouses_cbegin() const { return storehouses_.cbegin(); }
        NodeCollection<Storehouse>::const_iterator storehouses_cend() const { return storehouses_.cend(); }

    private:
        void remove_receiver(NodeCollection<Node>& collection, ElementID id) {};

        // te metody powinny być publiczne:
        bool is_consistent() {};
        void do_deliveries(Time t) {};
        void do_package_passing() {};
        void do_work(Time t) {};

        NodeCollection<Ramp> ramps_;
        NodeCollection<Worker> workers_;
        NodeCollection<Storehouse> storehouses_;
};

class NodeCollection {

};

#endif