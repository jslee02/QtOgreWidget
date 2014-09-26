#include "QOgreWidget.hpp"

#include <string>
#include <cassert>

#include <QResizeEvent>

//#ifdef __unix__
// #include <QtX11Extras/QX11Info>
//#endif

//------------------------------------------------------------------------------
QOgreWidget::QOgreWidget(Ogre::Root* ogreRoot,
                         IOgreEventHandler* ogreEventHandler,
                         QWidget* parent)
  : QGLWidget(parent),
    mOgreEventHandler(ogreEventHandler),
    mOgreRoot(ogreRoot),
    mOgreRenderWindow(NULL)
{
  if (!parent)
  {
    // interface requirement
    throw std::runtime_error("Parent widget supplied was uninitialised!");
  }

  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NoSystemBackground, true);
  setFocusPolicy(Qt::StrongFocus);

  Ogre::String winHandle;
#ifdef WIN32
  // Windows code
  winHandle += Ogre::StringConverter::toString(this->parentWidget()->winId());
#else
  // winHandle = Ogre::StringConverter::toString((unsigned long)(QX11Info::display()));
  // winHandle += ":";
  // winHandle += Ogre::StringConverter::toString((unsigned int)(QX11Info::appScreen()));
  // winHandle += ":";
  winHandle += Ogre::StringConverter::toString((unsigned long)(winId()));
#endif

  Ogre::NameValuePairList params;
  params["parentWindowHandle"] = winHandle;
  params["FSAA"] = Ogre::String("8");

  int w = width();
  int h = height();
  mOgreRenderWindow = mOgreRoot->createRenderWindow("OgreWidget_RenderWindow",
                                                    qMax(w, 640), qMax(h, 480),
                                                    false, &params);
  mOgreRenderWindow->setActive(true);
  mOgreRenderWindow->setVisible(true);

  WId ogreWinId = 0x0;
  mOgreRenderWindow->getCustomAttribute("WINDOW", &ogreWinId);

  // Guaranteed to be valid due to the way it was created
  assert(ogreWinId);

  QWidget::create(ogreWinId);

  // Qt won't bother updating the area under/behind the render window
  setAttribute(Qt::WA_OpaquePaintEvent);
}

//------------------------------------------------------------------------------
Ogre::RenderWindow *QOgreWidget::getEmbeddedOgreWindow()
{
  // Guaranteed to be valid after construction
  assert(mOgreRenderWindow);

  return mOgreRenderWindow;
}

//------------------------------------------------------------------------------
void QOgreWidget::paintEvent(QPaintEvent *pEvent)
{
  this->update();
}

//------------------------------------------------------------------------------
void QOgreWidget::resizeEvent(QResizeEvent *rEvent)
{
  if (rEvent)
    QWidget::resizeEvent(rEvent);

  if (mOgreRenderWindow)
  {
    // Since the underlying widget has already been updated we can source the
    // resize values from there
    mOgreRenderWindow->reposition(x(), y());
    mOgreRenderWindow->resize(width(), height());
    mOgreRenderWindow->windowMovedOrResized();
  }
}

//------------------------------------------------------------------------------
void QOgreWidget::mousePressEvent(QMouseEvent *event)
{
  mOgreEventHandler->ogreMousePressEvent(event);
}

//------------------------------------------------------------------------------
void QOgreWidget::mouseMoveEvent(QMouseEvent *event)
{
  mOgreEventHandler->ogreMouseMoveEvent(event);
}

//------------------------------------------------------------------------------
void QOgreWidget::update()
{
  QWidget::update();
  mOgreRoot->renderOneFrame();
}
