#include <QCompleter>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStringListModel>

#include "Common/CPath.hpp"

#include "GUI/Client/ClientRoot.hpp"
#include "GUI/Client/SelectPathDialog.hpp"

#include "GUI/Client/GraphicalUrl.hpp"

using namespace CF::Common;
using namespace CF::GUI::Client;

GraphicalUrl::GraphicalUrl(QWidget *parent) :
    GraphicalValue(parent)
{
  m_btBrowse = new QPushButton("Browse", this);
  m_editPath = new QLineEdit(this);
  m_completerModel = new QStringListModel(this);
  m_completer = new QCompleter(this);

  m_editPath->setCompleter(m_completer);
  m_completer->setModel(m_completerModel);

  m_layout->addWidget(m_editPath);
  m_layout->addWidget(m_btBrowse);

  connect(m_btBrowse, SIGNAL(clicked()), this, SLOT(btBrowseClicked()));
  connect(m_editPath, SIGNAL(textChanged(QString)), this, SLOT(updateModel(QString)));

  this->updateModel("");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

GraphicalUrl::~GraphicalUrl()
{
  delete m_btBrowse;
  delete m_editPath;
  delete m_completer;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QVariant GraphicalUrl::getValue() const
{
  return m_editPath->text();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool GraphicalUrl::setValue(const QVariant & path)
{
  m_originalValue = path;
  m_editPath->setText(path.toString());
  return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GraphicalUrl::btBrowseClicked()
{
  SelectPathDialog spd;

  CPath path = spd.show(m_editPath->text().toStdString());

  if(!path.empty())
    m_editPath->setText(path.string().c_str());
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void GraphicalUrl::updateModel(const QString & path)
{
  int lastSlash = path.lastIndexOf(CPath::separator().c_str());
  QString newPath;
  QStringList list;
  CNode::Ptr node;

  CRoot::Ptr root = ClientRoot::getTree()->getRoot()->root();

  try
  {
    node = root->access_component<CNode>(path.toStdString());
  }
  catch(InvalidPath & ip) {}

  if(node != CFNULL)
    newPath = path;
  else
    newPath = path.left(lastSlash);

  if(newPath == CPath::separator().c_str())
    newPath = root->full_path().string().c_str();

  try
  {
    if(root->full_path().string() == newPath.toStdString())
      node = ClientRoot::getTree()->getRoot();
    else
      node = root->access_component<CNode>(newPath.toStdString());

    if(node != CFNULL)
    {
      QStringList list;
      node->listChildPaths(list, false, false);

      node = boost::dynamic_pointer_cast<CNode>(node->get_parent());

      if(node != CFNULL)
        node->listChildPaths(list, false, false);

      m_completerModel->setStringList(list);
    }
  }
  catch(InvalidPath & ip) {}

  emit valueChanged();
}
