
class Worker: public IPackageReceiver, public PackageSender{

    public: 
        Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q)
    :PackageSender(), id_(id), processing_duration_(pd), queue_(std::move(q)) {}

    // Dla potomnych:
    //  - powyżej konstruktor który wywołuje też konstruktor odpowiedniej instancji PackageSender
    //  - przyjąłem konwencję że do identyfikacji Workera służy typ wyliczeniowy i jego ID (gettery poniżej)
    //  - jeszcze nie wiem po jaką cholerę te const_iteratory ale kazali to zrobiłem
    //  - pola prywatne mają rzeczy które mogą obchodzić tylko mnie
    //  - najważniejsze jest d0_work()
    //      *SZYMON: zerknij w implementację jak będziesz pisał PackageSender
    //  - ADAM:
    //      *ogarnij sobie na jakich metodach mam override jak będziesz interfejs pisał
    //      *jak będziesz pisał typy wyliczeniowe to trzeba dodać jeszcze jakiś do indentyfikacji odbiorców przesyłki
    //       (patrz: metoda get_receiver_type() poniżej)
    //                                              ~ Krzychu 24.01 A.D.2026

        void do_work(Time t);
        TimeOffset get_processing_duration() const {return processing_duration_; }
        Time get_package_processing_start() const;
        ElementID get_id() const override { return id_; }
        ReceiverType get_receiver_type() const override { return ReceiverType::WORKER; }

        void receive_package(Package&& package) override {
            queue_->push(std::move(package));
        }

        IPackageStockpile cbegin() const override { return queue_->cbegin(); }
        IPackageStockpile cend() const override { return queue_->cend(); }

    private:
        TimeOffset processing_duration_;
        Time processing_start = 0;
        ElementID id_;

        std::unique_ptr<IPackageQueue> queue_;
        std::optional<Package> current_package = std::nullopt;

};