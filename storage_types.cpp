#include "storage_types.hxx"
#include <stdexcept>

Package PackageQueue::pop() {
    if (data.empty()) {
        throw std::out_of_range("Ta kolejka jest pusta, nie można usunąć pakietu.");
    }

    // Jakby co, to tutaj wybiera pakiet typu do zwrócenia
    // w zależności od typu wyliczeniowego (FIFO/LIFO)
    Package package_to_return = std::move(
        (type == PackageQueueType::FIFO) ? data.front() : data.back()
    );

    // A jak się przeniesie dane to jeszcze trzeba usunąć
    // miejsce po tych danych z listy 
    if (type == PackageQueueType::FIFO) {
        data.pop_front();
    } else {
        data.pop_back();
    }

    return package_to_return;
}