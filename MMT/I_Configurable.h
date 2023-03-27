#ifndef I_CONFIGURABLE_H
#define I_CONFIGURABLE_H

class I_Configurable
{
public:
    virtual void openConfiguration() = 0;
    virtual void closeConfiguration() = 0;
    virtual void applyConfiguration() = 0;
};

#endif // I_CONFIGURABLE_H
