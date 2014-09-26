#ifndef QOGREWIDGETDEMO_H_
#define QOGREWIDGETDEMO_H_

#include <OGRE/Ogre.h>

#include <QMainWindow>
#include <QCameraMan.h>

#include "IOgreEventHandler.hpp"

class QOgreWidget;

/// class QOgreWidgetDemo
class QOgreWidgetDemo : public QMainWindow, public IOgreEventHandler
{
  Q_OBJECT

public:

  /// Constructor
  QOgreWidgetDemo();

  /// Destructor
  ~QOgreWidgetDemo();

  ///
  void setupRenderSystem();

  ///
  void setupResources();

  ///
  void createQtWidgets();

  ///
  void createScene();

private:

  ///
  QOgreWidget* mOgreWidget;

  ///
  Ogre::Root* mOgreRoot;

  ///
  Ogre::RenderWindow* mRenderWindow;

  ///
  Ogre::Viewport* mOgreViewport;

  ///
  Ogre::RenderSystem* mRenderSystem;

  ///
  Ogre::SceneManager* mSceneManager;

  ///
  Ogre::Camera* mCamera;

  ///
  QCameraMan* mCameraMan;

  ///
  int mMouseMoveXOld;

  ///
  int mMouseMoveYOld;

private slots:

  //----------------------------------------------------------------------------
  // Button event handler
  //----------------------------------------------------------------------------

  ///
  void onZoomIn();

  ///
  void onZoomOut();

  //----------------------------------------------------------------------------
  // IQOgreEventHandler
  //----------------------------------------------------------------------------

  ///
  virtual void ogrePaintEvent(QPaintEvent* _event) {}

  ///
  virtual void ogreResizeEvent(QResizeEvent* _event) {}

  ///
  virtual void ogreUpdate() {}

  ///
  virtual void ogreMousePressEvent(QMouseEvent* _event);

  ///
  virtual void ogreMouseMoveEvent(QMouseEvent* _event);
};

#endif // QTOGREAPPLICATION_H_
