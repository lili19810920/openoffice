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



package ifc.reflection;

import lib.MultiMethodTest;

import com.sun.star.lang.XInitialization;
import com.sun.star.reflection.XProxyFactory;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XAggregation;

/**
/**
* Testing <code>com.sun.star.reflection.XProxyFactory</code>
* interface methods :
* <ul>
*  <li><code> createProxy()</code></li>
* </ul> <p>
* Test is <b> NOT </b> multithread compilant. <p>
* @see com.sun.star.reflection.XProxyFactory
*/
public class _XProxyFactory extends MultiMethodTest {
    /** Is initialized in super class(using reflection API)
     * when instantiating the test.
     */
    public XProxyFactory oObj;

    /**
    * First an implementation of
    * <code>com.sun.star.lang.XInitialization</code> interface
    * is made which sets a flag when its <code>initialize()</code>
    * method is called. Then an instance of this implementation
    * is created and a proxy object is created for it. Proxy
    * object is tried to query for <code>XInitialization</code>
    * interface and it's <code>initialize</code> method is
    * called. The goal is to check if the real object method
    * was called throwgh it's proxy. <p>
    * Has <b>OK</b> status if the real object method was
    * called and paramters were passed correctly.
    */
    public void _createProxy() {
        class MyObject implements XInitialization {
            Object[] params;

            public void initialize(Object args[]) {
                params = args;
            }
        }

        MyObject obj = new MyObject();

        XAggregation xAggr = oObj.createProxy(obj);

        XInitialization xInit = (XInitialization)UnoRuntime.queryInterface(
                XInitialization.class, xAggr);

        Object params[] = new Object[0];

        try {
            xInit.initialize(params);
        } catch(com.sun.star.uno.Exception e) {
            log.println("Unexpected exception : " + e.getMessage());
            e.printStackTrace(log);
            tRes.tested("createProxy()", false);
            return;
        }

        tRes.tested("createProxy()",
            util.ValueComparer.equalValue(params, obj.params));
    }
}

