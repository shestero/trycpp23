#include "filelist.h"

#include <QDir>

FileList::FileList()
{
        QDir directory(".");
        QStringList files =
                directory.entryList(
                    QStringList() << "*.ssd" << "*.SSD",
                    QDir::Files,
                    QDir::Name  // sort order
                );
        addItems(files);
}
