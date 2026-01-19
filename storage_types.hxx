enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:
        virtual ~IPackageStockpile() = default;
        virtual ~IPackageStockpile() = default;
        virtual void push(const Package& package) = 0;
        virtual const_iterator begin() const = 0;
        virtual const_iterator end() const = 0;

        IPackageStockpile() = default;  
};
