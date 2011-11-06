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



package mod._ucprmt;

import java.io.PrintWriter;

import lib.StatusException;
import lib.TestCase;
import lib.TestEnvironment;
import lib.TestParameters;

import com.sun.star.lang.XMultiServiceFactory;
import com.sun.star.ucb.XRemoteContentProviderAcceptor;
import com.sun.star.uno.UnoRuntime;
import com.sun.star.uno.XInterface;

public class ProviderAcceptor extends TestCase {

    public TestEnvironment createTestEnvironment
            (TestParameters Param, PrintWriter log) {

        XInterface oObj = null;
        Object oInterface = null;

        try {
            oInterface = ((XMultiServiceFactory)Param.getMSF()).createInstance
                ("com.sun.star.ucb.RemoteContentProviderAcceptor");
        } catch( com.sun.star.uno.Exception e ) {
            log.println("Can't create an object." );
            throw new StatusException( "Can't create an object", e );
        }

        oObj = (XInterface) oInterface;

        TestEnvironment tEnv = new TestEnvironment( oObj );

        //adding ObjRelation for XRemoteContentProviderAcceptor
        XRemoteContentProviderAcceptor xRCPA = (XRemoteContentProviderAcceptor)
            UnoRuntime.queryInterface(XRemoteContentProviderAcceptor.class,oObj);

        tEnv.addObjRelation("RCPA",xRCPA);

        return tEnv;
    } // finish method getTestEnvironment

}

