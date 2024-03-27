/*******************************************************************************
 * Copyright (c) 2001-2020 Rohit Priyadarshi
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 * https://opensource.org/licenses/MIT
 ******************************************************************************/

#include <math.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <bitset>
#include <sstream>
#include <assert.h>
#include <mutex>
#include <algorithm>

#include <json/json.h>
#include <gtest/gtest.h>

#include "racksys.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                               //
//       ###############################################################################################         //
//       #      ###################################           ###################################      #         //
//       #      #                      cell_3     #           #                      cell_3     #      #         //
//       #      #    ##########     ##########    #           #    ##########     ##########    #      #         //
//       #      # n1 # cell_1 # n4  # cell_2 # n5 # n6top     # n1 # cell_1 # n4  # cell_2 # n5 #      # n7top   //
// top_a 0------O----O A    Z O-----O A    Z O----O-----+-----O----O A    Z O-----O A    Z O----O------0 top_y   //
//       #    A # n2 #        #     #        #    # Z   |   A # n2 #        #     #        #    # Z    #         //
// top_b 0------O----O B      #  +--O B      #    # +---------O----O B      #  +--O B      #    #      #         //
//       #    B #    # inst_1 #  |  # inst_2 #    # |   |   B #    # inst_1 #  |  # inst_2 #    #      #         //
//       #      # n3 ##########  |  ##########    # |   |     # n3 ##########  #  ##########    #      #         //
// top_c 0------O----------------+                # | +-------O----------------+                #      #         //
//       #    C #                          inst_3 # | | |   C #                          inst_4 #      #         //
//       #      ################################### | | |     ###################################      #         //
//       # n4top                                    | | |                                              #         //
// top_d 0------------------------------------------+ | |                                              #         //
//       # n5top                                    | | |                                              #         //
// top_e 0--------------------------------------------+ |                                              #         //
//       #                                          | | |     ###################################      #         //
//       #                                          | | |     #                      cell_3     #      #         //
//       #                                          | | |     #    ##########     ##########    #      #         //
//       #                                          | | |     # n1 # cell_1 # n4  # cell_2 # n5 #      # n8top   //
//       #                                          | | +-----O----O A    Z O-----O A    Z O----O------0 top_z   //
//       #                                          | |     A # n2 #        #     #        #    # Z    #         //
//       #                                          +-|-------O----O B      #  +--O B      #    #      #         //
//       #                                            |     B #    # inst_1 #  |  # inst_2 #    #      #         //
//       #                                            |       # n3 ##########  #  ##########    #      #         //
//       #                                            +-------O----------------+                #      #         //
//       #                                                  C #                          inst_5 #      #         //
//       #                                                    ###################################      #         //
//       ###############################################################################################         //
//                                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string srack("rack");
utl::multistring mrack(srack);
class rackTest : public testing::Test
{
public:
    rackTest() : m_srack("rack"), m_mrack(m_srack), m_rack(m_mrack) {}

protected:
    rack::Rack& rack() { return m_rack; }
    void createRack()
    {
        // Create the Containers in RACK.
        rack().createTechnology("technology");
        rack().createLibrary("library");
        rack().createDesign("design");
        rack().createHdl("hdl");
    }
    void createCell_1(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);
        rack::Module& m = d->createModule(name, rack());

        // Create available views for the module.
        std::string viewName("default");
        rack::ViewManager& vm = m.viewManager();
        vm.createBlackbox(viewName, rack());
        vm.createPhysical(viewName, rack());
        vm.createTiming(viewName, rack());
        vm.createCell(viewName, rack());
        vm.createHdl(viewName, rack());
        vm.createViewGroup(viewName, rack());

        // Create ports on the module.
        m.createPort("A", rack::mInput, rack());
        m.createPort("B", rack::mInput, rack());
        m.createPort("Z", rack::mOutput, rack());
    }
    void verifyCell_1(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        std::stringstream ss;
        ss << m->name();
        EXPECT_STREQ(name.c_str(), ss.str().c_str());

        std::string viewName("default");
        rack::ViewManager& vm = m->viewManager();
        ASSERT_TRUE(vm.findBlackbox(viewName));
        ASSERT_TRUE(vm.findPhysical(viewName));
        ASSERT_TRUE(vm.findTiming(viewName));
        ASSERT_TRUE(vm.findCell(viewName));
        ASSERT_TRUE(vm.findHdl(viewName));
        ASSERT_TRUE(vm.findViewGroup(viewName));

        // Ports
        ASSERT_TRUE(m->findPort("A", rack()));
        ASSERT_TRUE(m->findPort("B", rack()));
        ASSERT_TRUE(m->findPort("Z", rack()));
    }
    void createCell_2(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);
        rack::Module& m = d->createModule(name, rack());

        std::string viewName("default");
        // Create available views for the module.
        rack::ViewManager& vm = m.viewManager();
        vm.createBlackbox(viewName, rack());
        vm.createPhysical(viewName, rack());
        vm.createTiming(viewName, rack());
        vm.createCell(viewName, rack());
        vm.createHdl(viewName, rack());
        vm.createViewGroup(viewName, rack());

        // Create ports on the module.
        m.createPort("A", rack::mInput, rack());
        m.createPort("B", rack::mInput, rack());
        m.createPort("Z", rack::mOutput, rack());
    }
    void verifyCell_2(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        std::stringstream ss;
        ss << m->name();
        EXPECT_STREQ(name.c_str(), ss.str().c_str());

        std::string viewName("default");
        rack::ViewManager& vm = m->viewManager();
        ASSERT_TRUE(vm.findBlackbox(viewName));
        ASSERT_TRUE(vm.findPhysical(viewName));
        ASSERT_TRUE(vm.findTiming(viewName));
        ASSERT_TRUE(vm.findCell(viewName));
        ASSERT_TRUE(vm.findHdl(viewName));
        ASSERT_TRUE(vm.findViewGroup(viewName));

        // Ports
        ASSERT_TRUE(m->findPort("A", rack()));
        ASSERT_TRUE(m->findPort("B", rack()));
        ASSERT_TRUE(m->findPort("Z", rack()));
    }
    void createCell_3(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);
        rack::Module& m = d->createModule(name, rack());

        std::string viewName("default");
        // Create available views for the module.
        rack::ViewManager& vm = m.viewManager();
        vm.createBlackbox(viewName, rack());
        vm.createPhysical(viewName, rack());
        vm.createNetlist(viewName, rack());
        vm.createTiming(viewName, rack());
        vm.createHdl(viewName, rack());
        vm.createViewGroup(viewName, rack());

        // Create ports on the module.
        m.createPort("A", rack::mInput, rack());
        m.createPort("B", rack::mInput, rack());
        m.createPort("C", rack::mInput, rack());
        m.createPort("Z", rack::mOutput, rack());
    }
    void buildNetlistCell_3(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        std::string viewName("default");
        rack::Module* m1 = d->findModule("cell_1", rack());
        ASSERT_TRUE(m1);
        rack::ViewManager& vm1 = m1->viewManager();
        rack::Rack::_ViewGroup* vgm1 = vm1.findViewGroup(viewName);
        ASSERT_TRUE(vgm1);

        rack::Module* m2 = d->findModule("cell_2", rack());
        ASSERT_TRUE(m2);
        rack::ViewManager& vm2 = m2->viewManager();
        rack::Rack::_ViewGroup* vgm2 = vm2.findViewGroup(viewName);
        ASSERT_TRUE(vgm2);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        rack::ViewManager& vm = m->viewManager();
        rack::Rack::_ViewGroup* vgm = vm.findViewGroup(viewName);
        ASSERT_TRUE(vgm);

        // Create instances in the module.
        rack::Netlist* nl = vm.findNetlist(viewName);
        ASSERT_TRUE(nl);
        rack::Instance& i1 = nl->createInstance("inst_1", *vgm, *vgm1, rack());
        rack::Instance& i2 = nl->createInstance("inst_2", *vgm, *vgm2, rack());

        // Create Nets.
        rack::Net& n1 = nl->createNet("n1", rack());
        rack::Net& n2 = nl->createNet("n2", rack());
        rack::Net& n3 = nl->createNet("n3", rack());
        rack::Net& n4 = nl->createNet("n4", rack());
        rack::Net& n5 = nl->createNet("n5", rack());

        // Make connections.
        i1.connect("A", n1, rack());
        i1.connect("B", n2, rack());
        i1.connect("Z", n4, rack());

        i2.connect("A", n4, rack());
        i2.connect("B", n3, rack());
        i2.connect("Z", n5, rack());

        m->connect("A", n1, rack());
        m->connect("B", n2, rack());
        m->connect("C", n3, rack());
        m->connect("Z", n5, rack());
    }
    void verifyCell_3(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        std::stringstream ss;
        ss << m->name();
        EXPECT_STREQ(name.c_str(), ss.str().c_str());

        std::string viewName("default");
        rack::ViewManager& vm = m->viewManager();
        ASSERT_TRUE(vm.findBlackbox(viewName));
        ASSERT_TRUE(vm.findPhysical(viewName));
        ASSERT_TRUE(vm.findTiming(viewName));
        ASSERT_TRUE(vm.findNetlist(viewName));
        ASSERT_TRUE(vm.findHdl(viewName));
        ASSERT_TRUE(vm.findViewGroup(viewName));

        // Ports
        rack::Connector* porta = m->findPort("A", rack());
        ASSERT_TRUE(porta);
        rack::Connector* portb = m->findPort("B", rack());
        ASSERT_TRUE(portb);
        rack::Connector* portc = m->findPort("C", rack());
        ASSERT_TRUE(portc);
        rack::Connector* portz = m->findPort("Z", rack());
        ASSERT_TRUE(portz);

        // rack::Netlist
        rack::Netlist* nl = vm.findNetlist(viewName);
        ASSERT_TRUE(nl);

        // Instances
        rack::InstanceBase* i1 = nl->findInstance("inst_1", rack());
        ASSERT_TRUE(i1);
        rack::InstanceBase* i2 = nl->findInstance("inst_2", rack());
        ASSERT_TRUE(i2);

        // Nets
        rack::NetBase* n1 = nl->findNet("n1", rack());
        ASSERT_TRUE(n1);
        rack::NetBase* n2 = nl->findNet("n2", rack());
        ASSERT_TRUE(n2);
        rack::NetBase* n3 = nl->findNet("n3", rack());
        ASSERT_TRUE(n3);
        rack::NetBase* n4 = nl->findNet("n4", rack());
        ASSERT_TRUE(n4);
        rack::NetBase* n5 = nl->findNet("n5", rack());
        ASSERT_TRUE(n5);

        // Cross-find: instance->pin
        rack::Connector* i1a = i1->findPin("A", rack());
        ASSERT_TRUE(i1a);
        rack::Connector* i1b = i1->findPin("B", rack());
        ASSERT_TRUE(i1b);
        rack::Connector* i1z = i1->findPin("Z", rack());
        ASSERT_TRUE(i1z);

        rack::Connector* i2a = i2->findPin("A", rack());
        ASSERT_TRUE(i2a);
        rack::Connector* i2b = i2->findPin("B", rack());
        ASSERT_TRUE(i2b);
        rack::Connector* i2z = i2->findPin("Z", rack());
        ASSERT_TRUE(i2z);

        // Cross-find: net->pin
        // These should be sets instead of pointers due to duplicate names
        // Make sure that the collection can deal with duplicate keys
        rack::Connector* n1pina = n1->findPins("A", rack()).first->second;
        ASSERT_TRUE(n1pina);
        ASSERT_TRUE(i1a == n1pina);

        rack::Connector* n1porta = n1->findPort("A", rack());
        ASSERT_TRUE(n1porta);
        ASSERT_TRUE(porta == n1porta);

        rack::Connector* n2pinb = n2->findPins("B", rack()).first->second;
        ASSERT_TRUE(n2pinb);
        ASSERT_TRUE(i1b == n2pinb);

        rack::Connector* n2portb = n2->findPort("B", rack());
        ASSERT_TRUE(n2portb);
        ASSERT_TRUE(portb == n2portb);

        rack::Connector* n3pinb = n3->findPins("B", rack()).first->second;
        ASSERT_TRUE(n3pinb);
        ASSERT_TRUE(i2b == n3pinb);

        rack::Connector* n3portc = n3->findPort("C", rack());
        ASSERT_TRUE(n3portc);
        ASSERT_TRUE(portc == n3portc);

        rack::Connector* n4pina = n4->findPins("A", rack()).first->second;
        ASSERT_TRUE(n4pina);
        ASSERT_TRUE(i2a == n4pina);

        rack::Connector* n4pinz = n4->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n4pinz);
        ASSERT_TRUE(i1z == n4pinz);

        rack::Connector* n5pinz = n5->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n5pinz);
        ASSERT_TRUE(i2z == n5pinz);

        rack::Connector* n5portz = n5->findPort("Z", rack());
        ASSERT_TRUE(n5portz);
        ASSERT_TRUE(portz == n5portz);
    }
    void createTop(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);
        rack::Module& m = d->createModule(name, rack());

        std::string viewName("default");
        // Create available views for the module.
        rack::ViewManager& vm = m.viewManager();
        vm.createBlackbox(viewName, rack());
        vm.createPhysical(viewName, rack());
        vm.createNetlist(viewName, rack());
        vm.createTiming(viewName, rack());
        vm.createTiming("RACK_1", rack());
        vm.createHdl(viewName, rack());
        vm.createViewGroup(viewName, rack());

        // Create ports on the module.
        m.createPort("top_a", rack::mInput, rack());
        m.createPort("top_b", rack::mInput, rack());
        m.createPort("top_c", rack::mInput, rack());
        m.createPort("top_d", rack::mInput, rack());
        m.createPort("top_e", rack::mInput, rack());
        m.createPort("top_y", rack::mOutput, rack());
        m.createPort("top_z", rack::mOutput, rack());
    }
    void buildNetlistTop(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        std::string viewName("default");
        rack::Module* m3 = d->findModule("cell_3", rack());
        ASSERT_TRUE(m3);
        rack::ViewManager& vm3 = m3->viewManager();
        rack::Rack::_ViewGroup* vgm3 = vm3.findViewGroup(viewName);
        ASSERT_TRUE(vgm3);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        rack::ViewManager& vm = m->viewManager();
        rack::Rack::_ViewGroup* vgm = vm.findViewGroup(viewName);
        ASSERT_TRUE(vgm);

        // Create instances in the module.
        rack::Netlist* nl = vm.findNetlist(viewName);
        ASSERT_TRUE(nl);
        rack::Instance& i1 = nl->createInstance("inst_3", *vgm, *vgm3, rack());
        rack::Instance& i2 = nl->createInstance("inst_4", *vgm, *vgm3, rack());
        rack::Instance& i3 = nl->createInstance("inst_5", *vgm, *vgm3, rack());

        // Create Nets.
        rack::Net& n1 = nl->createNet("n1top", rack());
        rack::Net& n2 = nl->createNet("n2top", rack());
        rack::Net& n3 = nl->createNet("n3top", rack());
        rack::Net& n4 = nl->createNet("n4top", rack());
        rack::Net& n5 = nl->createNet("n5top", rack());
        rack::Net& n6 = nl->createNet("n6top", rack());
        rack::Net& n7 = nl->createNet("n7top", rack());
        rack::Net& n8 = nl->createNet("n8top", rack());

        // Make connections.
        i1.connect("A", n1, rack());
        i1.connect("B", n2, rack());
        i1.connect("C", n3, rack());
        i1.connect("Z", n6, rack());

        i2.connect("A", n6, rack());
        i2.connect("B", n4, rack());
        i2.connect("C", n5, rack());
        i2.connect("Z", n7, rack());

        i3.connect("A", n6, rack());
        i3.connect("B", n4, rack());
        i3.connect("C", n5, rack());
        i3.connect("Z", n8, rack());

        m->connect("top_a", n1, rack());
        m->connect("top_b", n2, rack());
        m->connect("top_c", n3, rack());
        m->connect("top_d", n4, rack());
        m->connect("top_e", n5, rack());
        m->connect("top_y", n7, rack());
        m->connect("top_z", n8, rack());
    }
    void verifyTop(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        std::stringstream ss;
        ss << m->name();
        EXPECT_STREQ(name.c_str(), ss.str().c_str());

        std::string viewName("default");
        rack::ViewManager& vm = m->viewManager();
        ASSERT_TRUE(vm.findBlackbox(viewName));
        ASSERT_TRUE(vm.findPhysical(viewName));
        ASSERT_TRUE(vm.findTiming(viewName));
        ASSERT_TRUE(vm.findTiming("RACK_1"));
        ASSERT_TRUE(vm.findNetlist(viewName));
        ASSERT_TRUE(vm.findHdl(viewName));
        ASSERT_TRUE(vm.findViewGroup(viewName));

        // Ports
        rack::Connector* porta = m->findPort("top_a", rack());
        ASSERT_TRUE(porta);
        rack::Connector* portb = m->findPort("top_b", rack());
        ASSERT_TRUE(portb);
        rack::Connector* portc = m->findPort("top_c", rack());
        ASSERT_TRUE(portc);
        rack::Connector* portd = m->findPort("top_d", rack());
        ASSERT_TRUE(portd);
        rack::Connector* porte = m->findPort("top_e", rack());
        ASSERT_TRUE(porte);
        rack::Connector* porty = m->findPort("top_y", rack());
        ASSERT_TRUE(porty);
        rack::Connector* portz = m->findPort("top_z", rack());
        ASSERT_TRUE(portz);

        // rack::Netlist
        rack::Netlist* nl = vm.findNetlist(viewName);
        ASSERT_TRUE(nl);

        // Instances
        rack::InstanceBase* i1 = nl->findInstance("inst_3", rack());
        ASSERT_TRUE(i1);
        rack::InstanceBase* i2 = nl->findInstance("inst_4", rack());
        ASSERT_TRUE(i2);
        rack::InstanceBase* i3 = nl->findInstance("inst_5", rack());
        ASSERT_TRUE(i3);

        // Nets
        rack::NetBase* n1 = nl->findNet("n1top", rack());
        ASSERT_TRUE(n1);
        rack::NetBase* n2 = nl->findNet("n2top", rack());
        ASSERT_TRUE(n2);
        rack::NetBase* n3 = nl->findNet("n3top", rack());
        ASSERT_TRUE(n3);
        rack::NetBase* n4 = nl->findNet("n4top", rack());
        ASSERT_TRUE(n4);
        rack::NetBase* n5 = nl->findNet("n5top", rack());
        ASSERT_TRUE(n5);
        rack::NetBase* n6 = nl->findNet("n6top", rack());
        ASSERT_TRUE(n6);
        rack::NetBase* n7 = nl->findNet("n7top", rack());
        ASSERT_TRUE(n7);
        rack::NetBase* n8 = nl->findNet("n8top", rack());
        ASSERT_TRUE(n8);

        // Cross-find: instance->pin
        rack::Connector* i1a = i1->findPin("A", rack());
        ASSERT_TRUE(i1a);
        rack::Connector* i1b = i1->findPin("B", rack());
        ASSERT_TRUE(i1b);
        rack::Connector* i1c = i1->findPin("C", rack());
        ASSERT_TRUE(i1c);
        rack::Connector* i1z = i1->findPin("Z", rack());
        ASSERT_TRUE(i1z);

        rack::Connector* i2a = i2->findPin("A", rack());
        ASSERT_TRUE(i2a);
        rack::Connector* i2b = i2->findPin("B", rack());
        ASSERT_TRUE(i2b);
        rack::Connector* i2c = i2->findPin("C", rack());
        ASSERT_TRUE(i2c);
        rack::Connector* i2z = i2->findPin("Z", rack());
        ASSERT_TRUE(i2z);

        rack::Connector* i3a = i3->findPin("A", rack());
        ASSERT_TRUE(i3a);
        rack::Connector* i3b = i3->findPin("B", rack());
        ASSERT_TRUE(i3b);
        rack::Connector* i3c = i3->findPin("C", rack());
        ASSERT_TRUE(i3c);
        rack::Connector* i3z = i3->findPin("Z", rack());
        ASSERT_TRUE(i3z);

        // Cross-find: net->pin
        // These should be sets instead of pointers due to duplicate names
        // Make sure that the collection can deal with duplicate keys
        rack::Connector* n1pina = n1->findPins("A", rack()).first->second;
        ASSERT_TRUE(n1pina);
        ASSERT_TRUE(i1a == n1pina);

        rack::Connector* n1porta = n1->findPort("top_a", rack());
        ASSERT_TRUE(n1porta);
        ASSERT_TRUE(porta == n1porta);

        rack::Connector* n2pinb = n2->findPins("B", rack()).first->second;
        ASSERT_TRUE(n2pinb);
        ASSERT_TRUE(i1b == n2pinb);

        rack::Connector* n2portb = n2->findPort("top_b", rack());
        ASSERT_TRUE(n2portb);
        ASSERT_TRUE(portb == n2portb);

        rack::Connector* n3pinc = n3->findPins("C", rack()).first->second;
        ASSERT_TRUE(n3pinc);
        ASSERT_TRUE(i1c == n3pinc);

        rack::Connector* n3portc = n3->findPort("top_c", rack());
        ASSERT_TRUE(n3portc);
        ASSERT_TRUE(portc == n3portc);

        rack::ConnectorMultiItrPair n4pinb = n4->findPins("B", rack());
        rack::ConnectorMultiItr in4pinb = n4pinb.first;

        rack::Connector* n4pinb1 = in4pinb->second;
        ASSERT_TRUE(n4pinb1);
        ASSERT_TRUE(i2b == n4pinb1);

        rack::Connector* n4pinb2 = (++in4pinb)->second;
        ASSERT_TRUE(n4pinb2);
        ASSERT_TRUE(i3b == n4pinb2);

        ASSERT_TRUE((++in4pinb) == n4pinb.second);

        rack::Connector* n4portd = n4->findPort("top_d", rack());
        ASSERT_TRUE(n4portd);
        ASSERT_TRUE(portd == n4portd);

        rack::ConnectorMultiItrPair n5pinc = n5->findPins("C", rack());
        rack::ConnectorMultiItr in5pinc = n5pinc.first;

        rack::Connector* n5pinc1 = in5pinc->second;
        ASSERT_TRUE(n5pinc1);
        ASSERT_TRUE(i2c == n5pinc1);

        rack::Connector* n5pinc2 = (++in5pinc)->second;
        ASSERT_TRUE(n5pinc2);
        ASSERT_TRUE(i3c == n5pinc2);

        ASSERT_TRUE((++in5pinc) == n5pinc.second);

        rack::Connector* n5porte = n5->findPort("top_e", rack());
        ASSERT_TRUE(n5porte);
        ASSERT_TRUE(porte == n5porte);

        rack::ConnectorMultiItrPair n6pina = n6->findPins("A", rack());
        rack::ConnectorMultiItr in6pina = n6pina.first;

        rack::Connector* n6pina1 = in6pina->second;
        ASSERT_TRUE(n6pina1);
        ASSERT_TRUE(i2a == n6pina1);

        rack::Connector* n6pina2 = (++in6pina)->second;
        ASSERT_TRUE(n6pina2);
        ASSERT_TRUE(i3a == n6pina2);

        ASSERT_TRUE((++in6pina) == n6pina.second);

        rack::Connector* n6pinz = n6->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n6pinz);
        ASSERT_TRUE(i1z == n6pinz);

        rack::Connector* n7pinz = n7->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n7pinz);
        ASSERT_TRUE(i2z == n7pinz);

        rack::Connector* n7porty = n7->findPort("top_y", rack());
        ASSERT_TRUE(n7porty);
        ASSERT_TRUE(porty == n7porty);

        rack::Connector* n8pinz = n8->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n8pinz);
        ASSERT_TRUE(i3z == n8pinz);

        rack::Connector* n8portz = n8->findPort("top_z", rack());
        ASSERT_TRUE(n8portz);
        ASSERT_TRUE(portz == n8portz);
    }
    void dissolveTop(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        rack::ViewManager& vm = m->viewManager();
        rack::Netlist* nl = vm.findNetlist("default");
        ASSERT_TRUE(nl);

        rack::InstanceBase* i3 = nl->findInstance("inst_3", rack());
        ASSERT_TRUE(i3);
        rack::InstanceBase* i4 = nl->findInstance("inst_4", rack());
        ASSERT_TRUE(i4);
        rack::InstanceBase* i5 = nl->findInstance("inst_5", rack());
        ASSERT_TRUE(i5);

        i3->dissolve(rack());
        i4->dissolve(rack());
        i5->dissolve(rack());
        delete i3;
        delete i4;
        delete i5;
    }
    void verifyFlatTop(const std::string& name)
    {
        rack::Design* d = rack().findDesign("design");
        ASSERT_TRUE(d);

        rack::Module* m = d->findModule(name, rack());
        ASSERT_TRUE(m);
        std::stringstream ss;
        ss << m->name();
        EXPECT_STREQ(name.c_str(), ss.str().c_str());

        std::string viewName("default");
        rack::ViewManager& vm = m->viewManager();
        ASSERT_TRUE(vm.findBlackbox(viewName));
        ASSERT_TRUE(vm.findPhysical(viewName));
        ASSERT_TRUE(vm.findTiming(viewName));
        ASSERT_TRUE(vm.findTiming("RACK_1"));
        ASSERT_TRUE(vm.findNetlist(viewName));
        ASSERT_TRUE(vm.findHdl(viewName));
        ASSERT_TRUE(vm.findViewGroup(viewName));

        // Ports
        rack::Connector* porta = m->findPort("top_a", rack());
        ASSERT_TRUE(porta);
        rack::Connector* portb = m->findPort("top_b", rack());
        ASSERT_TRUE(portb);
        rack::Connector* portc = m->findPort("top_c", rack());
        ASSERT_TRUE(portc);
        rack::Connector* portd = m->findPort("top_d", rack());
        ASSERT_TRUE(portd);
        rack::Connector* porte = m->findPort("top_e", rack());
        ASSERT_TRUE(porte);
        rack::Connector* porty = m->findPort("top_y", rack());
        ASSERT_TRUE(porty);
        rack::Connector* portz = m->findPort("top_z", rack());
        ASSERT_TRUE(portz);

        // rack::Netlist
        rack::Netlist* nl = vm.findNetlist(viewName);
        ASSERT_TRUE(nl);

        // Base names
        std::string inst_1("inst_1");
        std::string inst_2("inst_2");
        std::string inst_3("inst_3");
        std::string inst_4("inst_4");
        std::string inst_5("inst_5");

        // Hierarchical names
        utl::multistring im31(inst_3);
        im31 += inst_1;
        utl::multistring im41(inst_4);
        im41 += inst_1;
        utl::multistring im51(inst_5);
        im51 += inst_1;
        utl::multistring im32(inst_3);
        im32 += inst_2;
        utl::multistring im42(inst_4);
        im42 += inst_2;
        utl::multistring im52(inst_5);
        im52 += inst_2;

        // Instances
        rack::InstanceBase* i31 = nl->findInstance(im31, rack());
        ASSERT_TRUE(i31);
        rack::InstanceBase* i41 = nl->findInstance(im41, rack());
        ASSERT_TRUE(i41);
        rack::InstanceBase* i51 = nl->findInstance(im51, rack());
        ASSERT_TRUE(i51);
        rack::InstanceBase* i32 = nl->findInstance(im32, rack());
        ASSERT_TRUE(i32);
        rack::InstanceBase* i42 = nl->findInstance(im42, rack());
        ASSERT_TRUE(i42);
        rack::InstanceBase* i52 = nl->findInstance(im52, rack());
        ASSERT_TRUE(i52);

        // Nets
        rack::NetBase* n1 = nl->findNet("n1top", rack());
        ASSERT_TRUE(n1);
        rack::NetBase* n2 = nl->findNet("n2top", rack());
        ASSERT_TRUE(n2);
        rack::NetBase* n3 = nl->findNet("n3top", rack());
        ASSERT_TRUE(n3);
        rack::NetBase* n4 = nl->findNet("n4top", rack());
        ASSERT_TRUE(n4);
        rack::NetBase* n5 = nl->findNet("n5top", rack());
        ASSERT_TRUE(n5);
        rack::NetBase* n6 = nl->findNet("n6top", rack());
        ASSERT_TRUE(n6);
        rack::NetBase* n7 = nl->findNet("n7top", rack());
        ASSERT_TRUE(n7);
        rack::NetBase* n8 = nl->findNet("n8top", rack());
        ASSERT_TRUE(n8);

        std::string n_1("n1");
        std::string n_2("n2");
        std::string n_3("n3");
        std::string n_4("n4");
        std::string n_5("n5");

        // Hierarchical instance names
        utl::multistring nm31(inst_3);
        nm31 += n_1;
        utl::multistring nm41(inst_4);
        nm41 += n_1;
        utl::multistring nm51(inst_5);
        nm51 += n_1;
        utl::multistring nm32(inst_3);
        nm32 += n_2;
        utl::multistring nm42(inst_4);
        nm42 += n_2;
        utl::multistring nm52(inst_5);
        nm52 += n_2;
        utl::multistring nm33(inst_3);
        nm33 += n_3;
        utl::multistring nm43(inst_4);
        nm43 += n_3;
        utl::multistring nm53(inst_5);
        nm53 += n_3;
        utl::multistring nm34(inst_3);
        nm34 += n_4;
        utl::multistring nm44(inst_4);
        nm44 += n_4;
        utl::multistring nm54(inst_5);
        nm54 += n_4;
        utl::multistring nm35(inst_3);
        nm35 += n_5;
        utl::multistring nm45(inst_4);
        nm45 += n_5;
        utl::multistring nm55(inst_5);
        nm55 += n_5;

        rack::NetBase* n31 = nl->findNet(nm31, rack());
        ASSERT_TRUE(n31);
        rack::NetBase* n41 = nl->findNet(nm41, rack());
        ASSERT_TRUE(n41);
        rack::NetBase* n51 = nl->findNet(nm51, rack());
        ASSERT_TRUE(n51);

        rack::NetBase* n32 = nl->findNet(nm32, rack());
        ASSERT_TRUE(n32);
        rack::NetBase* n42 = nl->findNet(nm42, rack());
        ASSERT_TRUE(n42);
        rack::NetBase* n52 = nl->findNet(nm52, rack());
        ASSERT_TRUE(n52);

        rack::NetBase* n33 = nl->findNet(nm33, rack());
        ASSERT_TRUE(n33);
        rack::NetBase* n43 = nl->findNet(nm43, rack());
        ASSERT_TRUE(n43);
        rack::NetBase* n53 = nl->findNet(nm53, rack());
        ASSERT_TRUE(n53);

        rack::NetBase* n34 = nl->findNet(nm34, rack());
        ASSERT_TRUE(n34);
        rack::NetBase* n44 = nl->findNet(nm44, rack());
        ASSERT_TRUE(n44);
        rack::NetBase* n54 = nl->findNet(nm54, rack());
        ASSERT_TRUE(n54);

        rack::NetBase* n35 = nl->findNet(nm35, rack());
        ASSERT_TRUE(n35);
        rack::NetBase* n45 = nl->findNet(nm45, rack());
        ASSERT_TRUE(n45);
        rack::NetBase* n55 = nl->findNet(nm55, rack());
        ASSERT_TRUE(n55);

        // Cross-find: instance->pin
        rack::Connector* i31a = i31->findPin("A", rack());
        ASSERT_TRUE(i31a);
        rack::Connector* i31b = i31->findPin("B", rack());
        ASSERT_TRUE(i31b);
        rack::Connector* i31z = i31->findPin("Z", rack());
        ASSERT_TRUE(i31z);

        rack::Connector* i41a = i41->findPin("A", rack());
        ASSERT_TRUE(i41a);
        rack::Connector* i41b = i41->findPin("B", rack());
        ASSERT_TRUE(i41b);
        rack::Connector* i41z = i41->findPin("Z", rack());
        ASSERT_TRUE(i41z);

        rack::Connector* i51a = i51->findPin("A", rack());
        ASSERT_TRUE(i51a);
        rack::Connector* i51b = i51->findPin("B", rack());
        ASSERT_TRUE(i51b);
        rack::Connector* i51z = i51->findPin("Z", rack());
        ASSERT_TRUE(i51z);

        rack::Connector* i32a = i32->findPin("A", rack());
        ASSERT_TRUE(i32a);
        rack::Connector* i32b = i32->findPin("B", rack());
        ASSERT_TRUE(i32b);
        rack::Connector* i32z = i32->findPin("Z", rack());
        ASSERT_TRUE(i32z);

        rack::Connector* i42a = i42->findPin("A", rack());
        ASSERT_TRUE(i42a);
        rack::Connector* i42b = i42->findPin("B", rack());
        ASSERT_TRUE(i42b);
        rack::Connector* i42z = i42->findPin("Z", rack());
        ASSERT_TRUE(i42z);

        rack::Connector* i52a = i52->findPin("A", rack());
        ASSERT_TRUE(i52a);
        rack::Connector* i52b = i52->findPin("B", rack());
        ASSERT_TRUE(i52b);
        rack::Connector* i52z = i52->findPin("Z", rack());
        ASSERT_TRUE(i52z);

        // Cross-find: net->pin
        // These should be sets instead of pointers due to duplicate names
        // Make sure that the collection can deal with duplicate keys
        rack::Connector* n1pina = n1->findPins("A", rack()).first->second;
        ASSERT_TRUE(n1pina);
        ASSERT_TRUE(i31a == n1pina);

        rack::Connector* n1porta = n1->findPort("top_a", rack());
        ASSERT_TRUE(n1porta);
        ASSERT_TRUE(porta == n1porta);

        rack::Connector* n2pinb = n2->findPins("B", rack()).first->second;
        ASSERT_TRUE(n2pinb);
        ASSERT_TRUE(i31b == n2pinb);

        rack::Connector* n2portb = n2->findPort("top_b", rack());
        ASSERT_TRUE(n2portb);
        ASSERT_TRUE(portb == n2portb);

        rack::Connector* n34pinz = n34->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n34pinz);
        ASSERT_TRUE(i31z == n34pinz);

        rack::Connector* n34pina = n34->findPins("A", rack()).first->second;
        ASSERT_TRUE(n34pina);
        ASSERT_TRUE(i32a == n34pina);

        rack::Connector* n3pinb = n3->findPins("B", rack()).first->second;
        ASSERT_TRUE(n3pinb);
        ASSERT_TRUE(i32b == n3pinb);

        rack::Connector* n3portc = n3->findPort("top_c", rack());
        ASSERT_TRUE(n3portc);
        ASSERT_TRUE(portc == n3portc);

        // n55 is empty
        rack::Connector* n6pinz = n6->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n6pinz);
        ASSERT_TRUE(i32z == n6pinz);

        rack::ConnectorMultiItrPair n6pina = n6->findPins("A", rack());
        rack::ConnectorMultiItr in6pina = n6pina.first;

        rack::Connector* n6pina1 = in6pina->second;
        ASSERT_TRUE(n6pina1);
        ASSERT_TRUE(i41a == n6pina1);

        rack::Connector* n6pina2 = (++in6pina)->second;
        ASSERT_TRUE(n6pina2);
        ASSERT_TRUE(i51a == n6pina2);

        ASSERT_TRUE((++in6pina) == n6pina.second);

        rack::ConnectorMultiItrPair n4pinb = n4->findPins("B", rack());
        rack::ConnectorMultiItr in4pinb = n4pinb.first;

        rack::Connector* n4pinb1 = in4pinb->second;
        ASSERT_TRUE(n4pinb1);
        ASSERT_TRUE(i41b == n4pinb1);

        rack::Connector* n4pinb2 = (++in4pinb)->second;
        ASSERT_TRUE(n4pinb2);
        ASSERT_TRUE(i51b == n4pinb2);

        ASSERT_TRUE((++in4pinb) == n4pinb.second);

        rack::Connector* n4portd = n4->findPort("top_d", rack());
        ASSERT_TRUE(n4portd);
        ASSERT_TRUE(portd == n4portd);

        rack::ConnectorMultiItrPair n5pinb = n5->findPins("B", rack());
        rack::ConnectorMultiItr in5pinb = n5pinb.first;

        rack::Connector* n5pinb1 = in5pinb->second;
        ASSERT_TRUE(n5pinb1);
        ASSERT_TRUE(i42b == n5pinb1);

        rack::Connector* n5pinb2 = (++in5pinb)->second;
        ASSERT_TRUE(n5pinb2);
        ASSERT_TRUE(i52b == n5pinb2);

        ASSERT_TRUE((++in5pinb) == n5pinb.second);

        rack::Connector* n5porte = n5->findPort("top_e", rack());
        ASSERT_TRUE(n5porte);
        ASSERT_TRUE(porte == n5porte);

        rack::Connector* n44pinz = n44->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n44pinz);
        ASSERT_TRUE(i41z == n44pinz);

        rack::Connector* n44pina = n44->findPins("A", rack()).first->second;
        ASSERT_TRUE(n44pina);
        ASSERT_TRUE(i42a == n44pina);

        rack::Connector* n54pinz = n54->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n54pinz);
        ASSERT_TRUE(i51z == n54pinz);

        rack::Connector* n54pina = n54->findPins("A", rack()).first->second;
        ASSERT_TRUE(n54pina);
        ASSERT_TRUE(i52a == n54pina);

        rack::Connector* n7pinz = n7->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n7pinz);
        ASSERT_TRUE(i42z == n7pinz);

        rack::Connector* n7porty = n7->findPort("top_y", rack());
        ASSERT_TRUE(n7porty);
        ASSERT_TRUE(porty == n7porty);

        rack::Connector* n8pinz = n8->findPins("Z", rack()).first->second;
        ASSERT_TRUE(n8pinz);
        ASSERT_TRUE(i52z == n8pinz);

        rack::Connector* n8portz = n8->findPort("top_z", rack());
        ASSERT_TRUE(n8portz);
        ASSERT_TRUE(portz == n8portz);
    }
    virtual void SetUp()
    {
        try {
            createRack();
            createCell_1("cell_1");
            createCell_2("cell_2");
            createCell_3("cell_3");
            buildNetlistCell_3("cell_3");
            createTop("RACK");
            buildNetlistTop("RACK");
        }
        catch (utl::Exception& e) {
            std::cout << e << std::endl;
        }
    }

protected: // data
    std::string         m_srack;
    utl::multistring    m_mrack;
    rack::Rack        m_rack;
};

TEST_F(rackTest, defaultTechnology)
{
    ASSERT_TRUE(rack().defaultTechnology());
}
TEST_F(rackTest, defaultLibrary)
{
    ASSERT_TRUE(rack().defaultLibrary());
}

TEST_F(rackTest, defaultDesign)
{
    ASSERT_TRUE(rack().defaultDesign());
}

TEST_F(rackTest, defaultHdl)
{
    ASSERT_TRUE(rack().defaultHdl());
}

TEST_F(rackTest, findTechnology)
{
    std::string s("technology");
    rack::Rack::_Technology* t = rack().findTechnology(s);
    ASSERT_TRUE(t);
    std::stringstream ss;
    ss << t->name();
    EXPECT_STREQ(s.c_str(), ss.str().c_str());
}

TEST_F(rackTest, findLibrary)
{
    std::string s("library");
    rack::Rack::_Library* l = rack().findLibrary(s);
    ASSERT_TRUE(l);
    std::stringstream ss;
    ss << l->name();
    EXPECT_STREQ(s.c_str(), ss.str().c_str());
}

TEST_F(rackTest, findDesign)
{
    std::string s("design");
    rack::Rack::_Design* d = rack().findDesign(s);
    ASSERT_TRUE(d);
    std::stringstream ss;
    ss << d->name();
    EXPECT_STREQ(s.c_str(), ss.str().c_str());
}

TEST_F(rackTest, findHdl)
{
    std::string s("hdl");
    rack::Rack::_Hdl* h = rack().findHdl(s);
    ASSERT_TRUE(h);
    std::stringstream ss;
    ss << h->name();
    EXPECT_STREQ(s.c_str(), ss.str().c_str());
}

TEST_F(rackTest, verifyCell_1)
{
    verifyCell_1("cell_1");
}

TEST_F(rackTest, verifyCell_2)
{
    verifyCell_2("cell_2");
}

TEST_F(rackTest, verifyCell_3)
{
    verifyCell_3("cell_3");
}

TEST_F(rackTest, verifyTop)
{
    verifyTop("RACK");
}
TEST_F(rackTest, cloneCell_1)
{
    rack::Design* d = rack().findDesign("design");
    ASSERT_TRUE(d);

    rack::Module* m = d->findModule("cell_1", rack());
    m->clone("cell_1_clone", rack());
    verifyCell_1("cell_1_clone");
}
TEST_F(rackTest, cloneCell_2)
{
    rack::Design* d = rack().findDesign("design");
    ASSERT_TRUE(d);

    rack::Module* m = d->findModule("cell_2", rack());
    m->clone("cell_2_clone", rack());
    verifyCell_2("cell_2_clone");
}
TEST_F(rackTest, cloneCell_3)
{
    rack::Design* d = rack().findDesign("design");
    ASSERT_TRUE(d);

    rack::Module* m = d->findModule("cell_3", rack());
    m->clone("cell_3_clone", rack());
    verifyCell_3("cell_3_clone");
}
TEST_F(rackTest, cloneTop)
{
    rack::Design* d = rack().findDesign("design");
    ASSERT_TRUE(d);

    rack::Module* m = d->findModule("RACK", rack());
    m->clone("RACK_clone", rack());
    verifyTop("RACK_clone");
}
TEST_F(rackTest, dissolveTop)
{
    dissolveTop("RACK");
    verifyFlatTop("RACK");
}
#if 0
TEST_F(rackTest, dump)
{
    //std::cout << rack();
}

TEST_F(rackTest, xml)
{
    ofstream xml("rack.xml");
    DumpXML __xmlf(xml,
        "../../../../ext/XML/XSLTCkBkCode/text/tree-control-with-collapse.xslt",
        //"",
        "rack1.xsd");
    __xmlf.header();
    __xmlf.stylesheet();
    __xmlf << rack();
}

TEST_F(rackTest, verilog)
{
    ofstream ver("rack.v");
    DumpVerilog __verf(ver);
    __verf << rack();
}
#endif
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if 0
TEST(hierarchyTest, rack) {

    // Create the RACK.
    rack::std::std rack(mrack);
    try {
        // Create the Containers in RACK.
        rack.createTechnology("technology");
        rack.createLibrary("library");
        rack.createDesign("design");
        rack.createHdl("hdl");
        // Get design reference to enable creation of modules.
        rack::Rack::_DesignItr di = rack.designs().begin();
        rack::Design& des = *((*di).second);

        // Create first module.
        rack::Module& m1 = des.createModule("cell_1", rack);

        // Create available views for the module.
        rack::ViewManager& vm1 = m1.viewManager();
        vm1.createBlackbox("cell_1", rack);
        vm1.createPhysical("cell_1", rack);
        vm1.createTiming("cell_1", rack);
        vm1.createCell("cell_1", rack);
        vm1.createHdl("cell_1", rack);
        ViewGroup& vgm1 = vm1.createViewGroup("default", rack);

        // Create ports on the module.
        m1.createPort("A", rack::mInput, rack);
        m1.createPort("B", rack::mInput, rack);
        m1.createPort("Z", rack::mOutput, rack);

        // Create second module.
        rack::Module& m2 = des.createModule("cell_2", rack);

        // Create available views for the module.
        rack::ViewManager& vm2 = m2.viewManager();
        vm2.createBlackbox("cell_2", rack);
        vm2.createPhysical("cell_2", rack);
        //vm2.createNetlist("cell_2", rack);
        vm2.createTiming("cell_2", rack);
        vm2.createCell("cell_2", rack);
        vm2.createHdl("cell_2", rack);
        ViewGroup& vgm2 = vm2.createViewGroup("default", rack);

        // Create ports on the module.
        m2.createPort("A", rack::mInput, rack);
        m2.createPort("B", rack::mInput, rack);
        m2.createPort("Z", rack::mOutput, rack);

        // Create third module.
        rack::Module& m3 = des.createModule("cell_3", rack);

        // Create available views for the module.
        rack::ViewManager& vm3 = m3.viewManager();
        vm3.createBlackbox("cell_3", rack);
        vm3.createPhysical("cell_3", rack);
        vm3.createNetlist("cell_3", rack);
        vm3.createTiming("cell_3", rack);
        vm3.createHdl("cell_3", rack);
        ViewGroup& vgm3 = vm3.createViewGroup("default", rack);

        // Create ports on the module.
        m3.createPort("A", rack::mInput, rack);
        m3.createPort("B", rack::mInput, rack);
        m3.createPort("C", rack::mInput, rack);
        m3.createPort("Z", rack::mOutput, rack);

        // Create instances in the module.
        rack::Netlist* nl = vm3.findNetlist("cell_3");
        rack::Instance& i1 = nl->createInstance("inst_1", vgm3, vgm1, rack);
        rack::Instance& i2 = nl->createInstance("inst_2", vgm3, vgm2, rack);

        // Create Nets.
        rack::Net& n1 = nl->createNet("n1", rack);
        rack::Net& n2 = nl->createNet("n2", rack);
        rack::Net& n3 = nl->createNet("n3", rack);
        rack::Net& n4 = nl->createNet("n4", rack);
        rack::Net& n5 = nl->createNet("n5", rack);

        // Make connections.
        i1.connect("A", n1, rack);
        i1.connect("B", n2, rack);
        i1.connect("Z", n4, rack);

        i2.connect("A", n4, rack);
        i2.connect("B", n3, rack);
        i2.connect("Z", n5, rack);

        m3.connect("A", n1, rack);
        m3.connect("B", n2, rack);
        m3.connect("C", n3, rack);
        m3.connect("Z", n5, rack);

        // Create top module.
        rack::Module& top = des.createModule("RACK", rack);

        // Create available views for the module.
        rack::ViewManager& vtop = top.viewManager();
        vtop.assign(vtop);
        vtop.createBlackbox("RACK", rack);
        vtop.createPhysical("RACK", rack);
        vtop.createNetlist("RACK", rack);
        vtop.createTiming("RACK", rack);
        vtop.createTiming("RACK_1", rack);
        vtop.createHdl("RACK", rack);
        ViewGroup& vgtop = vtop.createViewGroup("default", rack);

        //ViewBase* pv1 = vtop.findTiming("RACK");
        //ViewBase* pv2 = vtop.findTiming("RACK_1");
        //ViewBase* pv3 = vtop.findTiming("bad");

        // Create ports on the module.
        top.createPort("top_a", rack::mInput, rack);
        top.createPort("top_b", rack::mInput, rack);
        top.createPort("top_c", rack::mInput, rack);
        top.createPort("top_d", rack::mInput, rack);
        top.createPort("top_e", rack::mInput, rack);
        top.createPort("top_z", rack::mOutput, rack);

        // Create instances in the module.
        rack::Netlist* nltop = vtop.findNetlist("RACK");
        rack::Instance& i1top = nltop->createInstance("inst_3", vgtop, vgm3, rack);
        rack::Instance& i2top = nltop->createInstance("inst_4", vgtop, vgm3, rack);

        // Create Nets.
        rack::Net& n1top = nltop->createNet("n1top", rack);
        rack::Net& n2top = nltop->createNet("n2top", rack);
        rack::Net& n3top = nltop->createNet("n3top", rack);
        rack::Net& n4top = nltop->createNet("n4top", rack);
        rack::Net& n5top = nltop->createNet("n5top", rack);
        rack::Net& n6top = nltop->createNet("n6top", rack);
        rack::Net& n7top = nltop->createNet("n7top", rack);

        // Make connections.
        i1top.connect("A", n1top, rack);
        i1top.connect("B", n2top, rack);
        i1top.connect("C", n3top, rack);
        i1top.connect("Z", n6top, rack);

        i2top.connect("A", n6top, rack);
        i2top.connect("B", n4top, rack);
        i2top.connect("C", n5top, rack);
        i2top.connect("Z", n7top, rack);

        top.connect("top_a", n1top, rack);
        top.connect("top_b", n2top, rack);
        top.connect("top_c", n3top, rack);
        top.connect("top_d", n4top, rack);
        top.connect("top_e", n5top, rack);
        top.connect("top_z", n7top, rack);
        
#if 0
        top.clone("topGun", rack);
        m1.clone("m1Gun", rack);
        m2.clone("m2Gun", rack);
        m3.clone("m3Gun", rack);
#endif

#if 0
#if 1
        //i1top.dissolve(rack);
        i2top.dissolve(rack);
#else
        ThreadGroup tg;
        tg.create_thread(
            InstanceRackThreadAdapter(
                rack::Instance::dissolve, i1top, rack));
        tg.join_all();
#endif
#endif
        DumpXML __xmlo(std::cout);
        __xmlo.header();
        __xmlo.stylesheet();
        std::cout << rack;

        ofstream xml("rack.xml");
        DumpXML __xmlf(xml, 
            "../../../../ext/XML/XSLTCkBkCode/text/tree-control-with-collapse.xslt", 
            //"",
            "rack1.xsd");
        __xmlf.header();
        __xmlf.stylesheet();
        //rack.dump(__xmlf);
        __xmlf << rack;

        ofstream ver("rack.v");
        DumpVerilog __verf(ver);
        //rack.dump(__verf);
        __verf << rack;

        int it = -12;
        FuncAdapter0 nx(&rack::Instance::navNext, i1top, rack);
        FuncAdapter0 cx(&rack::Instance::navCurr, i1top, rack);
        FuncAdapter0 px(&rack::Instance::navPrev, i1top, rack);
        FuncAdapter0 pax(&rack::Instance::navParent, i1top, rack);
        VoidFuncAdapter1 bax(&rack::Instance::backward, i1top, &i1top, rack);
        VoidFuncAdapter1 fwx(&rack::Instance::forward, i1top, &i1top, rack);
        VoidFuncAdapter1 st1x(&rack::Instance::stopat1, i1top, &i1top, rack);
        VoidFuncAdapter2 st2x(&rack::Instance::stopat2, i1top, &i1top, it, rack);
        VoidNavigatorNode2 nd2x(nx, cx, px, pax, fwx, bax, st2x);
        VoidNavigator2 nav2x(nd2x);

        nx();
        cx();
        px();
        pax();
        bax();
        fwx();
        st1x();
        st2x();

        FuncAdapter0 ny(&rack::Instance::navNext, rack);
        FuncAdapter0 cy(&rack::Instance::navCurr, rack);
        FuncAdapter0 py(&rack::Instance::navPrev, rack);
        FuncAdapter0 pay(&rack::Instance::navParent, rack);
        VoidFuncAdapter1 bay(&rack::Instance::backward, rack);
        VoidFuncAdapter1 fwy(&rack::Instance::forward, rack);
        VoidFuncAdapter1 st1y(&rack::Instance::stopat1, rack);
        VoidFuncAdapter2 st2y(&rack::Instance::stopat2, rack);

        ny(i1top);
        cy(i1top);
        py(i1top);
        pay(i1top);
        bay(i1top, &i1top);
        fwy(i1top, &i1top);
        st1y(i1top, &i1top);
        st2y(i1top, &i1top, it);

        FuncAdapter0 nz(&rack::Instance::navNext);
        FuncAdapter0 cz(&rack::Instance::navCurr);
        FuncAdapter0 pz(&rack::Instance::navPrev);
        FuncAdapter0 paz(&rack::Instance::navParent);
        VoidFuncAdapter1 baz(&rack::Instance::backward);
        VoidFuncAdapter1 fwz(&rack::Instance::forward);
        VoidFuncAdapter1 st1z(&rack::Instance::stopat1);
        VoidFuncAdapter2 st2z(&rack::Instance::stopat2);

        nz(i1top, rack);
        cz(i1top, rack);
        pz(i1top, rack);
        paz(i1top, rack);
        baz(i1top, &i1top, rack);
        fwz(i1top, &i1top, rack);
        st1z(i1top, &i1top, rack);
        st2z(i1top, &i1top, it, rack);

        VoidNavigatorNode1 vndx1(nx, cx, px, pax, fwx, bax, st1x);
        VoidNavigatorNode2 vndx2(nx, cx, px, pax, fwx, bax, st2x);
        VoidNavigatorNode1 vndy1(ny, cy, py, pay, fwy, bay, st1y);
        VoidNavigatorNode2 vndy2(ny, cy, py, pay, fwy, bay, st2y);
        VoidNavigatorNode1 vndz1(nz, cz, pz, paz, fwz, baz, st1z);
        VoidNavigatorNode2 vndz2(nz, cz, pz, paz, fwz, baz, st2z);

        vndx1.next()();
        vndx1.curr()();
        vndx1.prev()();
        vndx1.parent()();
        vndx1.forward()();
        vndx1.backward()();
        vndx1.stopat()();

        vndx2.next()();
        vndx2.curr()();
        vndx2.prev()();
        vndx2.parent()();
        vndx2.forward()();
        vndx2.backward()();
        vndx2.stopat()();

        vndy1.next()(i1top);
        vndy1.curr()(i1top);
        vndy1.prev()(i1top);
        vndy1.parent()(i1top);
        vndy1.forward()(i1top, &i1top);
        vndy1.backward()(i1top, &i1top);
        vndy1.stopat()(i1top, &i1top);

        vndy2.next()(i1top);
        vndy2.curr()(i1top);
        vndy2.prev()(i1top);
        vndy2.parent()(i1top);
        vndy2.forward()(i1top, &i1top);
        vndy2.backward()(i1top, &i1top);
        vndy2.stopat()(i1top, &i1top, it);

        vndz1.next()(i1top, rack);
        vndz1.curr()(i1top, rack);
        vndz1.prev()(i1top, rack);
        vndz1.parent()(i1top, rack);
        vndz1.forward()(i1top, &i1top, rack);
        vndz1.backward()(i1top, &i1top, rack);
        vndz1.stopat()(i1top, &i1top, rack);

        vndz2.next()(i1top, rack);
        vndz2.curr()(i1top, rack);
        vndz2.prev()(i1top, rack);
        vndz2.parent()(i1top, rack);
        vndz2.forward()(i1top, &i1top, rack);
        vndz2.backward()(i1top, &i1top, rack);
        vndz2.stopat().apply(i1top, &i1top, it, rack);
    }

    catch (Exception& e) {
        std::cout << e << std::endl;
    }
}
#endif

// TO_DO
// Remove the named-entity-base from base class, and derive the derived 
// classes from named-entity-base class. class pin was missing pointer
// to class port, but needs to get rid of name, since it must get name 
// from port.

// rack::Instance representing an array needs to be designed. A net representing
// a but needs to be designed. Ports and pins need to follow the suite.
#if 0
int
main(int argc, char** argv) {
#if 0
#if 0
    const int num_threads = 4;
    boost::thread_group thrds;
    for (int i=0; i < num_threads; ++i)
        thrds.create_thread(&testHierarchy);
    thrds.join_all();
#else
    boost::thread thrd1(&testHierarchy);
    boost::thread thrd2(&testHierarchy);
    boost::thread thrd3(&testHierarchy);
    boost::thread thrd4(&testHierarchy);
    thrd1.join();
    thrd2.join();
    thrd3.join();
    thrd4.join();
#endif
#else
#if 1
    testHierarchy();
#else
    boost::thread thrd(&testHierarchy);
    thrd.join();
#endif
#endif
    char buf[32];
    std::cout << "Trying integer to std::string conversion..." << std::endl;
    std::cout << IntToStr<char>(1234567890, buf) << std::endl;
    std::cout << IntToStr<char>(-1234567890, buf) << std::endl;
    std::cout << "done!" << std::endl;
}
#endif
/* HOW TO for Multithreading.
void _dissolve(_Rack& __r) throw (_Exception) {
    // TBD: the module corresponding to the instance needs to be 
    // cloned if needed ~ if the reference count is greater than 1.
    _ThreadGroup __tg;
    if (refCount() > 1) {
        _Component* __p_c = instanceOf();
        _Component* __p_c_new = 
            dynamic_cast<_Component*>(__p_c->clone("JUNK", __r));
    }
    for(_ConnectorConstItr __itr = pins().begin(); 
        __itr != pins().end(); __itr++) {
        _Connector* __pinext = __itr->second;
        if (!__pinext) {
            // throw exception
            assert(0);
        }
        __tg.create_thread(
            ConnectorBaseRackThreadAdapter(
                _Connector::dissolve, *__pinext, __r));
    }
    __tg.join_all();
}
*/
