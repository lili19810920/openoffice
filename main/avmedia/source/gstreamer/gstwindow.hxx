/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#ifndef _GSTWINDOW_HXX
#define _GSTWINDOW_HXX

#include "gstcommon.hxx"
#include <cppuhelper/interfacecontainer.h>

#ifndef _COM_SUN_STAR_MEDIA_XPLAYERWINDOW_HDL_
#include "com/sun/star/media/XPlayerWindow.hdl"
#endif

namespace avmedia
{
namespace gst
{

class PlayerChildWindow;

// ---------------
// - Window -
// ---------------

class Player;

class Window : public ::cppu::WeakImplHelper2< ::com::sun::star::media::XPlayerWindow,
                                               ::com::sun::star::lang::XServiceInfo >
{
    friend class Player;

public:     Window( ::avmedia::gst::Player& rPlayer );
    ~Window();

    bool create( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& aArguments );

    // XPlayerWindow
    virtual void SAL_CALL update()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual sal_Bool SAL_CALL setZoomLevel( ::com::sun::star::media::ZoomLevel ZoomLevel )
     throw( ::com::sun::star::uno::RuntimeException );
    virtual ::com::sun::star::media::ZoomLevel SAL_CALL getZoomLevel()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL setPointerType( sal_Int32 nPointerType )
     throw( ::com::sun::star::uno::RuntimeException );

    // XWindow
    virtual void SAL_CALL setPosSize( sal_Int32 X,
                                      sal_Int32 Y,
                                      sal_Int32 Width,
                                      sal_Int32 Height,
                                      sal_Int16 Flags )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual ::com::sun::star::awt::Rectangle SAL_CALL getPosSize()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL setVisible( sal_Bool Visible )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL setEnable( sal_Bool Enable )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL setFocus()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addWindowListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeWindowListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindowListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addFocusListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XFocusListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeFocusListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XFocusListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addKeyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XKeyListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeKeyListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XKeyListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addMouseListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeMouseListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addMouseMotionListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseMotionListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeMouseMotionListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XMouseMotionListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addPaintListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XPaintListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removePaintListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XPaintListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    // XComponent
    virtual void SAL_CALL dispose()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL addEventListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& xListener )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual void SAL_CALL removeEventListener(
        const ::com::sun::star::uno::Reference< ::com::sun::star::lang::XEventListener >& aListener )
     throw( ::com::sun::star::uno::RuntimeException );

    // XServiceInfo
    virtual ::rtl::OUString SAL_CALL getImplementationName()
     throw( ::com::sun::star::uno::RuntimeException );

    virtual sal_Bool SAL_CALL supportsService( const ::rtl::OUString& ServiceName )
     throw( ::com::sun::star::uno::RuntimeException );

    virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames()
     throw( ::com::sun::star::uno::RuntimeException );


public:

    void fireMousePressedEvent( const ::com::sun::star::awt::MouseEvent& rEvt );

    void fireMouseReleasedEvent( const ::com::sun::star::awt::MouseEvent& rEvt );

    void fireMouseMovedEvent( const ::com::sun::star::awt::MouseEvent& rEvt );

    void fireKeyPressedEvent( const ::com::sun::star::awt::KeyEvent& rEvt );

    void fireKeyReleasedEvent( const ::com::sun::star::awt::KeyEvent& rEvt );

    void fireSetFocusEvent( const ::com::sun::star::awt::FocusEvent& rEvt );

protected:

    long getXWindowHandle() const;

private:

    ::osl::Mutex maMutex;
    ::cppu::OMultiTypeInterfaceContainerHelper maListeners;
    ::com::sun::star::awt::Rectangle maWindowRect;
    Player& mrPlayer;
    PlayerChildWindow* mpPlayerWindow;
    ::com::sun::star::media::ZoomLevel meZoomLevel;
    sal_Int32 mnPointerType;

    void implLayoutVideoWindow();
};
} // namespace gst
} // namespace avmedia

#endif // _GSTWINDOW_HXX
