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



package mod._svx;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;
import util.AccessibilityTools;
import util.DrawTools;
import util.SOfficeFactory;
import util.utils;

import com.sun.star.accessibility.AccessibleRole;
import com.sun.star.accessibility.XAccessible;
import com.sun.star.awt.Size;
import com.sun.star.awt.XWindow;
import com.sun.star.beans.XPropertySet;
import com.sun.star.drawing.XShape;
import com.sun.star.frame.XModel;
import com.sun.star.lang.XComponent;
import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

public class AccessiblePresentationGraphicShape extends TestCase {

    static XComponent xDoc;
    static XModel aModel;

    protected void initialize( TestParameters tParam, PrintWriter log ) {

        SOfficeFactory SOF = SOfficeFactory.getFactory( (XMultiServiceFactory)tParam.getMSF() );

        try {
            log.println( "creating a drawdoc" );
            xDoc = SOF.createImpressDoc(null);
            aModel = (XModel)
                UnoRuntime.queryInterface(XModel.class, xDoc);

        } catch ( com.sun.star.uno.Exception e ) {
            // Some exception occures.FAILED
            e.printStackTrace( log );
            throw new StatusException( "Couldn't create document", e );
        }
    }

    /**
     * Disposes the Draw document loaded before.
     */
    protected void cleanup( TestParameters tParam, PrintWriter log ) {
        log.println( "    disposing xDrawDoc " );
        util.DesktopTools.closeDoc(xDoc);
    }

    protected TestEnvironment createTestEnvironment
            (TestParameters tParam, PrintWriter log) {

        XInterface oObj = null;
        XShape oShape = null;

        // creation of testobject here
        // first we write what we are intend to do to log file
        log.println( "creating a test environment" );
        
        XMultiServiceFactory docMSF = (XMultiServiceFactory)
            UnoRuntime.queryInterface(XMultiServiceFactory.class, xDoc);
        try {
            oShape = (XShape) UnoRuntime.queryInterface(XShape.class,
                docMSF.createInstance
                ("com.sun.star.presentation.GraphicObjectShape"));
        } catch (com.sun.star.uno.Exception e) {
            throw new StatusException("couldn't create component", e);
        }

        DrawTools.getShapes(DrawTools.getDrawPage(xDoc,0)).add(oShape);

        XPropertySet oShapeProps = (XPropertySet)
                            UnoRuntime.queryInterface(XPropertySet.class,oShape);
        try {
            oShapeProps.setPropertyValue(
                "GraphicURL",util.utils.getFullTestURL("space-metal.jpg"));
            oShapeProps.setPropertyValue("IsEmptyPresentationObject", new Boolean(false));
        } catch (com.sun.star.lang.WrappedTargetException e) {
        } catch (com.sun.star.lang.IllegalArgumentException e) {
        } catch (com.sun.star.beans.PropertyVetoException e) {
        } catch (com.sun.star.beans.UnknownPropertyException e) {
        }

        //DrawTools.getShapes(DrawTools.getDrawPage(xDoc,0)).add(oShape);

        AccessibilityTools at = new AccessibilityTools();

        XWindow xWindow = at.getCurrentWindow ((XMultiServiceFactory)tParam.getMSF(),aModel);
        XAccessible xRoot = at.getAccessibleObject(xWindow);

//        oObj = at.getAccessibleObjectForRole
//            (xRoot, AccessibleRole.SHAPE, "GraphicObject");
        oObj = at.getAccessibleObjectForRole(xRoot, AccessibleRole.UNKNOWN, "GraphicObject");

        // create test environment here
        TestEnvironment tEnv = new TestEnvironment( oObj );

        final XShape fShape = oShape ;
        tEnv.addObjRelation("EventProducer",
            new ifc.accessibility._XAccessibleEventBroadcaster.EventProducer() {
                public void fireEvent() {
                    try {
                        Size size = fShape.getSize();
                        size.Width += 100;
                        fShape.setSize(size);
                    } catch(com.sun.star.beans.PropertyVetoException e) {}
                }
            });

        log.println("Implementation Name: " + utils.getImplName(oObj));

        return tEnv;
    } // finish method getTestEnvironment

}

