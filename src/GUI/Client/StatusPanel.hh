#ifndef CF_GUI_Client_StatusPanel_h
#define CF_GUI_Client_StatusPanel_h

#include <QTreeView>

/////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace GUI {
namespace Client {
   
/////////////////////////////////////////////////////////////////////////////

  class StatusModel;
  
//////////////////////////////////////////////////////////////////////////////
  
  class StatusPanel : public QTreeView
  {
    Q_OBJECT
    
    public:
    
    StatusPanel(StatusModel * model, QWidget * parent = NULL);
    
    ~StatusPanel();
    
    private slots:
    
    void subSystemAdded(const QModelIndex & index);
    
    private:
    
    StatusModel * m_model;
    
  }; // class StatusPanel
  
//////////////////////////////////////////////////////////////////////////////
  
} // namespace Client
} // namespace GUI 
} // namespace CF
  
//////////////////////////////////////////////////////////////////////////////

#endif // CF_GUI_Client_StatusPanel_h