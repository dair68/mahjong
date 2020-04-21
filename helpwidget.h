#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QTabWidget>

class HelpWidget : public QTabWidget {
public:
    //creates shisensho help dialog
    //@param parent - pointer to parent widget
    HelpWidget(QWidget* parent=nullptr);

private:
    //creates overview tab
    void createOverviewTab();

    //creates basics tab
    void createBasicsTab();

    //creates removing tiles tab
    void createRemoveTab();

    //creates special tiles tab
    void createSpecialTab();

    //creates game tools tab
    void createToolsTab();
};

#endif // HELPWIDGET_H
