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



package util;

import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.uno.XInterface;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.text.XTextFrame;
import com.sun.star.drawing.XShape;
import com.sun.star.awt.Size;
import com.sun.star.beans.XPropertySet;
/**
 * the class FrameDsc
 */
public class FrameDsc extends InstDescr {

    private Size size = null;
    private int height = 2000;
    private int width = 2000;
    private String name = null;
    private int autoheigth = 0;
    private int anchorType = 0;// bound at paragraph

    final String ifcName = "com.sun.star.text.XTextFrame";
    final String service = "com.sun.star.text.TextFrame";

    public FrameDsc() {
        initFrame();
    }

    public FrameDsc( int nHeight, int nWidth ) {
        height = nHeight;
        width = nWidth;
        initFrame();
    }

    public FrameDsc( String FrameName, int nHeight, int nWidth ) {
        name = FrameName;
        height = nHeight;
        width = nWidth;
        initFrame();
    }
    public String getName() {
        return name;
    }
    public String getIfcName() {
        return ifcName;
    }
    public String getService() {
        return service;
    }

    private void initFrame() {
        try {
            ifcClass = Class.forName( ifcName );
        }
        catch( ClassNotFoundException cnfE ) {
        }
    }
    public XInterface createInstance( XMultiServiceFactory docMSF ) {
        Object SrvObj = null;

        size = new Size();
        size.Height = height;
        size.Width = width;

        try {
            SrvObj = docMSF.createInstance( service );
        }
        catch( com.sun.star.uno.Exception cssuE ){
        }
        XShape shape = (XShape)UnoRuntime.queryInterface( XShape.class, SrvObj );
        try {
            shape.setSize(size);
        }
        catch( com.sun.star.beans.PropertyVetoException pvE ){
        }

        XTextFrame TF = (XTextFrame)UnoRuntime.queryInterface( ifcClass, SrvObj );

        XPropertySet oPropSet = (XPropertySet)
                        UnoRuntime.queryInterface( XPropertySet.class, SrvObj );


        try {
            oPropSet.setPropertyValue("AnchorType", new Integer(2));
        }
        catch( com.sun.star.beans.UnknownPropertyException upE ){
        }
        catch( com.sun.star.beans.PropertyVetoException pvE ){
        }
        catch( com.sun.star.lang.IllegalArgumentException iaE ){
        }
        catch( com.sun.star.lang.WrappedTargetException wtE ){
        }



        return TF;
    }
}
