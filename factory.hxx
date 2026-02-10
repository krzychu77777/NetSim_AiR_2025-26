#ifndef FACTORY_HXX
#define FACTORY_HXX

#include "nodes.hxx"
#include <stdexcept>
#include <map>
#include <vector>
#include <memory>
#include <list>
#include <algorithm>
#include <iostream>

template <typename Node>
class NodeCollection {
public:
    //using container_t = std::vector<std::unique_ptr<Node>>;
    using container_t = std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    NodeCollection() = default;
    NodeCollection(NodeCollection&&) = default;
    NodeCollection& operator=(NodeCollection&&) = default;
    NodeCollection(const NodeCollection&) = delete;
    NodeCollection& operator=(const NodeCollection&) = delete;

    void add(Node&& node) {
        //nodes.emplace_back(std::make_unique<Node>(std::move(node)));
        nodes.emplace_back(std::move(node));
    }

    void remove_by_id(ElementID id) {
        // nodes.erase(
        //     std::remove_if(nodes.begin(), nodes.end(),
        //         [id](const std::unique_ptr<Node>& node) {
        //             return node->get_id() == id;
        //         }),
        //     nodes.end()
        // );
        auto it = find_by_id(id);
        if (it != nodes.end()) {
            nodes.erase(it);
        }
    }

    iterator find_by_id(ElementID id) {
        return std::find_if(nodes.begin(), nodes.end(),
            [id](const Node& node) {
                return node.get_id() == id;
            });
    }

    const_iterator find_by_id(ElementID id) const {
        return std::find_if(nodes.cbegin(), nodes.cend(),
            [id](const Node& node) {
                return node.get_id() == id;
            });
    }

    iterator cbegin() { return nodes.begin(); }
    iterator cend() { return nodes.end(); }
    const_iterator cbegin() const { return nodes.cbegin(); }
    const_iterator cend() const { return nodes.cend(); }

    iterator begin() { return nodes.begin(); }
    iterator end() { return nodes.end(); }
    const_iterator begin() const { return nodes.begin(); }
    const_iterator end() const { return nodes.end(); }

private:
    container_t nodes;
};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };

class Factory {
    public:
        Factory() = default;
        Factory(Factory&&) = default;
        Factory& operator=(Factory&&) = default;
        ~Factory() = default;

        //========== RAMPS ================
        void add_ramp(Ramp&& ramp) { ramp_.add(std::move(ramp)); }
        void remove_ramp(ElementID id) { ramp_.remove_by_id(id); }
        NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return ramp_.find_by_id(id); }
        NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return ramp_.find_by_id(id); }
        NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return ramp_.cbegin(); }
        NodeCollection<Ramp>::const_iterator ramp_cend() const { return ramp_.cend(); }

        //========== WORKER ================
        void add_worker(Worker&& worker) { worker_.add(std::move(worker)); }
        void remove_worker(ElementID id);
        NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return worker_.find_by_id(id); }
        NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return worker_.find_by_id(id); }
        NodeCollection<Worker>::const_iterator worker_cbegin() const { return worker_.cbegin(); }
        NodeCollection<Worker>::const_iterator worker_cend() const { return worker_.cend(); }

        //======== STOREHOUSE ==============
        void add_storehouse(Storehouse&& storehouse) { storehouse_.add(std::move(storehouse)); }
        void remove_storehouse(ElementID id); 
        NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return storehouse_.find_by_id(id); }
        NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return storehouse_.find_by_id(id); }
        NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return storehouse_.cbegin(); }
        NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return storehouse_.cend(); }

        bool is_consistent();
        void do_deliveries(Time t);
        void do_package_passing();
        void do_work(Time t);

    private:
        template <typename Node>
        void remove_receiver(NodeCollection<Node>& collection, ElementID id) {collection.remove_by_id(id);}
        void remove_receiver_links(IPackageReceiver* receiver);
        
        NodeCollection<Ramp> ramp_;
        NodeCollection<Worker> worker_;
        NodeCollection<Storehouse> storehouse_;
};

Factory load_factory_structure(std::istream& is);
void save_factory_structure(const Factory& factory, std::ostream& os);

#endif