#ifndef I_FILEMANAGER_H
#define I_FILEMANAGER_H

#include <QString>

class I_FileManager
{
public:
    virtual bool hasChangesUnsaved() const = 0;
    virtual void saveDiagrams() = 0;
    virtual void loadDiagrams() = 0;
    virtual void setFileName(const QString& p_FileName) = 0;
    virtual const QString& getFileName() const = 0;
};

#endif // I_FILEMANAGER_H
