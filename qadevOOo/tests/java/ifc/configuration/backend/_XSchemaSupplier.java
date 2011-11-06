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


package ifc.configuration.backend;

import com.sun.star.configuration.backend.XSchema;
import com.sun.star.configuration.backend.XSchemaSupplier;

import lib.MultiMethodTest;
import util.XSchemaHandlerImpl;

public class _XSchemaSupplier extends MultiMethodTest {
    public XSchemaSupplier oObj;

    public void _getComponentSchema() {
        boolean res = true;
        XSchema aSchema = null;

        try {
            aSchema = oObj.getComponentSchema("org.openoffice.Office.Linguistic");
            res &= (aSchema != null);

            if (aSchema == null) {
                log.println("\treturned Layer is NULL -- FAILED");
            }

            res &= checkSchema(aSchema);
        } catch (com.sun.star.configuration.backend.BackendAccessException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
            res &= false;
        } catch (com.sun.star.lang.IllegalArgumentException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
            res &= false;
        }
        tRes.tested("getComponentSchema()",res);
    }
    
    protected boolean checkSchema(XSchema aSchema) {
        boolean res = false;
        XSchemaHandlerImpl xSchemaHandlerImpl = new XSchemaHandlerImpl();
        log.println("Checking for Exception in case of null argument");

        try {
            aSchema.readTemplates(null);
            log.println("NoException thrown for null argument -- FAILED");
        } catch (com.sun.star.lang.NullPointerException e) {
            log.println("Expected Exception -- OK");
            res = true;
        } catch (com.sun.star.lang.WrappedTargetException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
        } catch (com.sun.star.configuration.backend.MalformedDataException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
        }

        log.println(
                "checking readComponent with own XSchemaHandler implementation");

        try {
            aSchema.readComponent(xSchemaHandlerImpl);
            
            String implCalled = xSchemaHandlerImpl.getCalls();            
            int sc = implCalled.indexOf("startComponent");

            if (sc < 0) {
                log.println("startComponent wasn't called -- FAILED");
                res &= false;
            } else {
                log.println("startComponent was called -- OK");
                res &= true;
            }

            int ec = implCalled.indexOf("endComponent");

            if (ec < 0) {
                log.println("endComponent wasn't called -- FAILED");
                res &= false;
            } else {
                log.println("endComponent was called -- OK");
                res &= true;
            }
        } catch (com.sun.star.lang.NullPointerException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
            res &= false;
        } catch (com.sun.star.lang.WrappedTargetException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
            res &= false;
        } catch (com.sun.star.configuration.backend.MalformedDataException e) {
            log.println("Unexpected Exception (" + e + ") -- FAILED");
            res &= false;
        }

        return res;        
        
    }
}