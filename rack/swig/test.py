# Setup import from build location
# https:##docs.python.org#3#tutorial#modules.html
import unittest
import pyrack

###################################################################################################################
##                                                                                                               ##
##       ###############################################################################################         ##
##       #      ###################################           ###################################      #         ##
##       #      #                      cell_3     #           #                      cell_3     #      #         ##
##       #      #    ##########     ##########    #           #    ##########     ##########    #      #         ##
##       #      # n1 # cell_1 # n4  # cell_2 # n5 # n6top     # n1 # cell_1 # n4  # cell_2 # n5 #      # n7top   ##
## top_a 0------O----O A    Z O-----O A    Z O----O-----+-----O----O A    Z O-----O A    Z O----O------0 top_y   ##
##       #    A # n2 #        #     #        #    # Z   |   A # n2 #        #     #        #    # Z    #         ##
## top_b 0------O----O B      #  +--O B      #    # +---------O----O B      #  +--O B      #    #      #         ##
##       #    B #    # inst_1 #  |  # inst_2 #    # |   |   B #    # inst_1 #  |  # inst_2 #    #      #         ##
##       #      # n3 ##########  |  ##########    # |   |     # n3 ##########  #  ##########    #      #         ##
## top_c 0------O----------------+                # | +-------O----------------+                #      #         ##
##       #    C #                          inst_3 # | | |   C #                          inst_4 #      #         ##
##       #      ################################### | | |     ###################################      #         ##
##       # n4top                                    | | |                                              #         ##
## top_d 0------------------------------------------+ | |                                              #         ##
##       # n5top                                    | | |                                              #         ##
## top_e 0--------------------------------------------+ |                                              #         ##
##       #                                          | | |     ###################################      #         ##
##       #                                          | | |     #                      cell_3     #      #         ##
##       #                                          | | |     #    ##########     ##########    #      #         ##
##       #                                          | | |     # n1 # cell_1 # n4  # cell_2 # n5 #      # n8top   ##
##       #                                          | | +-----O----O A    Z O-----O A    Z O----O------0 top_z   ##
##       #                                          | |     A # n2 #        #     #        #    # Z    #         ##
##       #                                          +-|-------O----O B      #  +--O B      #    #      #         ##
##       #                                            |     B #    # inst_1 #  |  # inst_2 #    #      #         ##
##       #                                            |       # n3 ##########  #  ##########    #      #         ##
##       #                                            +-------O----------------+                #      #         ##
##       #                                                  C #                          inst_5 #      #         ##
##       #                                                    ###################################      #         ##
##       ###############################################################################################         ##
##                                                                                                               ##
###################################################################################################################

# https://wiki.documentfoundation.org/Development/Python_Unit_Tests
# THese classes cannot have _init_()
class SampleDesign(unittest.TestCase):
    def setUp(self):
        self.build()
    
    def tearDown(self):
        pass
    
    def build(self):
        self.createRack()
        self.createCell_1('cell_1')
        self.createCell_2('cell_2')
        self.createCell_3('cell_3')
        self.buildNetlistCell_3('cell_3')
        self.createTop('RACK')
        self.buildNetlistTop('RACK')
        
    def createRack(self):
        self._rack.createTechnology('technology')
        self._rack.createLibrary('library')
        self._rack.createDesign('design')
        self._rack.createHdl('hdl')
        
    def createCell_1(self, name):
        print('createCell_1')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        m = d.createModule(name, self._rack)
        
        viewName = 'default'
        # Create available views for the module.
        vm = m.viewManager()
        vm.createBlackbox(viewName, self._rack)
        vm.createPhysical(viewName, self._rack)
        vm.createTiming(viewName, self._rack)
        vm.createCell(viewName, self._rack)
        vm.createHdl(viewName, self._rack)
        vm.createViewGroup(viewName, self._rack)
        
        # Create ports on the module.
        m.createPort('A', pyrack.mInput, self._rack)
        m.createPort('B', pyrack.mInput, self._rack)
        m.createPort('Z', pyrack.mOutput, self._rack)
        
    def verifyCell_1(self, name):
        print('verifyCell_1')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)

        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)
        
        viewName = 'default'
        vm = m.viewManager()
        self.assertIsInstance(vm.findBlackbox(viewName), pyrack.clBlackbox)
        self.assertIsInstance(vm.findPhysical(viewName), pyrack.clPhysical)
        self.assertIsInstance(vm.findTiming(viewName), pyrack.clTiming)
        self.assertIsInstance(vm.findCell(viewName), pyrack.clCell)
        self.assertIsInstance(vm.findHdl(viewName), pyrack.clHdl)
        self.assertIsInstance(vm.findViewGroup(viewName), pyrack.clViewGroup)

        # Ports
        self.assertIsInstance(m.findPort('A', self._rack), pyrack.clPort)
        self.assertIsInstance(m.findPort('B', self._rack), pyrack.clPort)
        self.assertIsInstance(m.findPort('Z', self._rack), pyrack.clPort)

    def createCell_2(self, name):
        print('createCell_2')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        m = d.createModule(name, self._rack)
        
        viewName = 'default'
        # Create available views for the module.
        vm = m.viewManager()
        vm.createBlackbox(viewName, self._rack)
        vm.createPhysical(viewName, self._rack)
        vm.createTiming(viewName, self._rack)
        vm.createCell(viewName, self._rack)
        vm.createHdl(viewName, self._rack)
        vm.createViewGroup(viewName, self._rack)
        
        # Create ports on the module.
        m.createPort('A', pyrack.mInput, self._rack)
        m.createPort('B', pyrack.mInput, self._rack)
        m.createPort('Z', pyrack.mOutput, self._rack)

    def verifyCell_2(self, name):
        print('verifyCell_2')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)

        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)
        
        viewName = 'default'
        vm = m.viewManager()
        self.assertIsInstance(vm.findBlackbox(viewName), pyrack.clBlackbox)
        self.assertIsInstance(vm.findPhysical(viewName), pyrack.clPhysical)
        self.assertIsInstance(vm.findTiming(viewName), pyrack.clTiming)
        self.assertIsInstance(vm.findCell(viewName), pyrack.clCell)
        self.assertIsInstance(vm.findHdl(viewName), pyrack.clHdl)
        self.assertIsInstance(vm.findViewGroup(viewName), pyrack.clViewGroup)

        # Ports
        self.assertIsInstance(m.findPort('A', self._rack), pyrack.clPort)
        self.assertIsInstance(m.findPort('B', self._rack), pyrack.clPort)
        self.assertIsInstance(m.findPort('Z', self._rack), pyrack.clPort)

    def createCell_3(self, name):
        print('createCell_3')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        m = d.createModule(name, self._rack)
        
        viewName = 'default'
        # Create available views for the module.
        vm = m.viewManager()
        vm.createBlackbox(viewName, self._rack)
        vm.createPhysical(viewName, self._rack)
        vm.createNetlist(viewName, self._rack)
        vm.createTiming(viewName, self._rack)
        vm.createHdl(viewName, self._rack)
        vm.createViewGroup(viewName, self._rack)
        
        # Create ports on the module.
        m.createPort('A', pyrack.mInput, self._rack)
        m.createPort('B', pyrack.mInput, self._rack)
        m.createPort('C', pyrack.mInput, self._rack)
        m.createPort('Z', pyrack.mOutput, self._rack)
        
    def buildNetlistCell_3(self, name):
        print('buildNetlistCell_3')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        
        viewName = 'default'
        c1 = 'cell_1'
        m1 = d.findModule(c1, self._rack)
        self.assertIsInstance(m1, pyrack.clModule)
        self.assertEqual(m1.name().str(), c1)
        vm1 = m1.viewManager()
        vgm1 = vm1.findViewGroup(viewName)
        self.assertIsInstance(vgm1, pyrack.clViewGroup)

        c2 = 'cell_2'
        m2 = d.findModule(c2, self._rack)
        self.assertIsInstance(m2, pyrack.clModule)
        self.assertEqual(m2.name().str(), c2)
        vm2 = m2.viewManager()
        vgm2 = vm2.findViewGroup(viewName)
        self.assertIsInstance(vgm2, pyrack.clViewGroup)

        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)
        vm = m.viewManager()
        vgm = vm.findViewGroup(viewName)
        self.assertIsInstance(vgm, pyrack.clViewGroup)

        # Create instances in the module.
        nl = vm.findNetlist(viewName)
        self.assertIsInstance(nl, pyrack.clNetlist)
        i1 = nl.createInstance('inst_1', vgm, vgm1, self._rack)
        self.assertIsInstance(i1, pyrack.clInstance)
        i2 = nl.createInstance('inst_2', vgm, vgm2, self._rack)
        self.assertIsInstance(i2, pyrack.clInstance)

        # Create Nets.
        n1 = nl.createNet('n1', self._rack)
        self.assertIsInstance(n1, pyrack.clNet)
        n2 = nl.createNet('n2', self._rack)
        self.assertIsInstance(n2, pyrack.clNet)
        n3 = nl.createNet('n3', self._rack)
        self.assertIsInstance(n3, pyrack.clNet)
        n4 = nl.createNet('n4', self._rack)
        self.assertIsInstance(n4, pyrack.clNet)
        n5 = nl.createNet('n5', self._rack)
        self.assertIsInstance(n5, pyrack.clNet)

        # Make connections.
        i1.connect('A', n1, self._rack)
        i1.connect('B', n2, self._rack)
        i1.connect('Z', n4, self._rack)
 
        i2.connect('A', n4, self._rack)
        i2.connect('B', n3, self._rack)
        i2.connect('Z', n5, self._rack)

        m.connect('A', n1, self._rack)
        m.connect('B', n2, self._rack)
        m.connect('C', n3, self._rack)
        m.connect('Z', n5, self._rack)
        
    def verifyCell_3(self, name):
        print('verifyCell_3')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        
        viewName = 'default'
        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)

        viewName = 'default'
        vm = m.viewManager()
        self.assertIsInstance(vm.findBlackbox(viewName), pyrack.clBlackbox)
        self.assertIsInstance(vm.findPhysical(viewName), pyrack.clPhysical)
        self.assertIsInstance(vm.findTiming(viewName), pyrack.clTiming)
        self.assertIsInstance(vm.findNetlist(viewName), pyrack.clNetlist)
        self.assertIsInstance(vm.findHdl(viewName), pyrack.clHdl)
        self.assertIsInstance(vm.findViewGroup(viewName), pyrack.clViewGroup)

        # Ports
        porta = m.findPort('A', self._rack)
        self.assertIsInstance(porta, pyrack.clPort)
        portb = m.findPort('B', self._rack)
        self.assertIsInstance(portb, pyrack.clPort)
        portc = m.findPort('C', self._rack)
        self.assertIsInstance(portc, pyrack.clPort)
        portz = m.findPort('Z', self._rack)
        self.assertIsInstance(portz, pyrack.clPort)

        # rack::Netlist
        nl = vm.findNetlist(viewName)
        self.assertIsInstance(nl, pyrack.clNetlist)
         
        # Instances
        i1 = nl.findInstance('inst_1', self._rack)
        self.assertIsInstance(i1, pyrack.clInstance)
        i2 = nl.findInstance('inst_2', self._rack)
        self.assertIsInstance(i2, pyrack.clInstance)

        # Nets
        n1 = nl.findNet('n1', self._rack)
        self.assertIsInstance(n1, pyrack.clNet)
        n2 = nl.findNet('n2', self._rack)
        self.assertIsInstance(n2, pyrack.clNet)
        n3 = nl.findNet('n3', self._rack)
        self.assertIsInstance(n3, pyrack.clNet)
        n4 = nl.findNet('n4', self._rack)
        self.assertIsInstance(n4, pyrack.clNet)
        n5 = nl.findNet('n5', self._rack)
        self.assertIsInstance(n5, pyrack.clNet)
        
        # Cross-find: instance.pin
        i1a = i1.findPin('A', self._rack)
        self.assertIsInstance(i1a, pyrack.clPin)
        i1b = i1.findPin('B', self._rack)
        self.assertIsInstance(i1b, pyrack.clPin)
        i1z = i1.findPin('Z', self._rack)
        self.assertIsInstance(i1z, pyrack.clPin)

        i2a = i2.findPin('A', self._rack)
        self.assertIsInstance(i2a, pyrack.clPin)
        i2b = i2.findPin('B', self._rack)
        self.assertIsInstance(i2b, pyrack.clPin)
        i2z = i2.findPin('Z', self._rack)
        self.assertIsInstance(i2z, pyrack.clPin)

        # Cross-find: net.pin
        # These should be sets instead of pointers due to duplicate names
        # Make sure that the collection can deal with duplicate keys
        n1pina = n1.findPin('A', self._rack)
        self.assertIsInstance(n1pina, pyrack.clPin)
        self.assertEqual(i1a.name().str(), n1pina.name().str())
 
        n1porta = n1.findPort('A', self._rack)
        self.assertIsInstance(n1porta, pyrack.clPort)
        self.assertEqual(porta.name().str(), n1porta.name().str())
 
        n1pinb = n2.findPin('B', self._rack)
        self.assertIsInstance(n1pinb, pyrack.clPin)
        self.assertEqual(i1b.name().str(), n1pinb.name().str())
  
        n2portb = n2.findPort('B', self._rack)
        self.assertIsInstance(n2portb, pyrack.clPort)
        self.assertEqual(portb.name().str(), n2portb.name().str())
  
        n3pinb = n3.findPin('B', self._rack)
        self.assertIsInstance(n3pinb, pyrack.clPin)
        self.assertEqual(i2b.name().str(), n3pinb.name().str())
  
        n3portc = n3.findPort('C', self._rack)
        self.assertIsInstance(n3portc, pyrack.clPort)
        self.assertEqual(portc.name().str(), n3portc.name().str())
  
        n4pina = n4.findPin('A', self._rack)
        self.assertIsInstance(n4pina, pyrack.clPin)
        self.assertEqual(i2a.name().str(), n4pina.name().str())
  
        n4pinz = n4.findPin('Z', self._rack)
        self.assertIsInstance(n4pinz, pyrack.clPin)
        self.assertEqual(i1z.name().str(), n4pinz.name().str())
  
        n5pinz = n5.findPin('Z', self._rack)
        self.assertIsInstance(n5pinz, pyrack.clPin)
        self.assertEqual(i2z.name().str(), n5pinz.name().str())
  
        n5portz = n5.findPort('Z', self._rack)
        self.assertIsInstance(n5portz, pyrack.clPort)
        self.assertEqual(portz.name().str(), n5portz.name().str())
        
    def createTop(self, name):
        print('createTop')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
        m = d.createModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
 
        viewName = 'default'
        # Create available views for the module.
        vm = m.viewManager()
        vm.createBlackbox(viewName, self._rack)
        vm.createPhysical(viewName, self._rack)
        vm.createNetlist(viewName, self._rack)
        vm.createTiming(viewName, self._rack)
        vm.createTiming('RACK_1', self._rack)
        vm.createHdl(viewName, self._rack)
        vm.createViewGroup(viewName, self._rack)
 
        # Create ports on the module.
        m.createPort('top_a', pyrack.mInput, self._rack)
        m.createPort('top_b', pyrack.mInput, self._rack)
        m.createPort('top_c', pyrack.mInput, self._rack)
        m.createPort('top_d', pyrack.mInput, self._rack)
        m.createPort('top_e', pyrack.mInput, self._rack)
        m.createPort('top_y', pyrack.mOutput, self._rack)
        m.createPort('top_z', pyrack.mOutput, self._rack)
        
    def buildNetlistTop(self, name):
        print('buildNetlistTop')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
 
        viewName = 'default'
        m3 = d.findModule('cell_3', self._rack)
        self.assertIsInstance(m3, pyrack.clModule)
        self.assertEqual(m3.name().str(), 'cell_3')
        vm3 = m3.viewManager()
        vgm3 = vm3.findViewGroup(viewName)
        self.assertIsInstance(vgm3, pyrack.clViewGroup)
 
        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)
        vm = m.viewManager()
        vgm = vm.findViewGroup(viewName)
        self.assertIsInstance(vgm, pyrack.clViewGroup)
 
        # Create instances in the module.
        nl = vm.findNetlist(viewName)
        self.assertIsInstance(nl, pyrack.clNetlist)
        i1 = nl.createInstance('inst_3', vgm, vgm3, self._rack)
        i2 = nl.createInstance('inst_4', vgm, vgm3, self._rack)
        i3 = nl.createInstance('inst_5', vgm, vgm3, self._rack)
 
        # Create Nets.
        n1 = nl.createNet('n1top', self._rack)
        n2 = nl.createNet('n2top', self._rack)
        n3 = nl.createNet('n3top', self._rack)
        n4 = nl.createNet('n4top', self._rack)
        n5 = nl.createNet('n5top', self._rack)
        n6 = nl.createNet('n6top', self._rack)
        n7 = nl.createNet('n7top', self._rack)
        n8 = nl.createNet('n8top', self._rack)
 
        # Make connections.
        i1.connect('A', n1, self._rack)
        i1.connect('B', n2, self._rack)
        i1.connect('C', n3, self._rack)
        i1.connect('Z', n6, self._rack)
 
        i2.connect('A', n6, self._rack)
        i2.connect('B', n4, self._rack)
        i2.connect('C', n5, self._rack)
        i2.connect('Z', n7, self._rack)
 
        i3.connect('A', n6, self._rack)
        i3.connect('B', n4, self._rack)
        i3.connect('C', n5, self._rack)
        i3.connect('Z', n8, self._rack)
 
        m.connect('top_a', n1, self._rack)
        m.connect('top_b', n2, self._rack)
        m.connect('top_c', n3, self._rack)
        m.connect('top_d', n4, self._rack)
        m.connect('top_e', n5, self._rack)
        m.connect('top_y', n7, self._rack)
        m.connect('top_z', n8, self._rack)
        
    def verifyTop(self, name):
        print('verifyTop')
        d = self._rack.findDesign('design')
        self.assertIsInstance(d, pyrack.clDesign)
 
        m = d.findModule(name, self._rack)
        self.assertIsInstance(m, pyrack.clModule)
        self.assertEqual(m.name().str(), name)
 
        viewName = 'default'
        vm = m.viewManager()
        self.assertIsInstance(vm.findBlackbox(viewName), pyrack.clBlackbox)
        self.assertIsInstance(vm.findPhysical(viewName), pyrack.clPhysical)
        self.assertIsInstance(vm.findTiming(viewName), pyrack.clTiming)
        self.assertIsInstance(vm.findTiming('RACK_1'), pyrack.clTiming)
        self.assertIsInstance(vm.findNetlist(viewName), pyrack.clNetlist)
        self.assertIsInstance(vm.findHdl(viewName), pyrack.clHdl)
        self.assertIsInstance(vm.findViewGroup(viewName), pyrack.clViewGroup)
 
        # Ports
        porta = m.findPort('top_a', self._rack)
        self.assertIsInstance(porta, pyrack.clPort)
        portb = m.findPort('top_b', self._rack)
        self.assertIsInstance(portb, pyrack.clPort)
        portc = m.findPort('top_c', self._rack)
        self.assertIsInstance(portc, pyrack.clPort)
        portd = m.findPort('top_d', self._rack)
        self.assertIsInstance(portd, pyrack.clPort)
        porte = m.findPort('top_e', self._rack)
        self.assertIsInstance(porte, pyrack.clPort)
        porty = m.findPort('top_y', self._rack)
        self.assertIsInstance(porty, pyrack.clPort)
        portz = m.findPort('top_z', self._rack)
        self.assertIsInstance(portz, pyrack.clPort)
 
        # rack::Netlist
        nl = vm.findNetlist(viewName)
        self.assertIsInstance(nl, pyrack.clNetlist)
 
        # Instances
        i1 = nl.findInstance('inst_3', self._rack)
        self.assertIsInstance(i1, pyrack.clInstance)
        i2 = nl.findInstance('inst_4', self._rack)
        self.assertIsInstance(i2, pyrack.clInstance)
        i3 = nl.findInstance('inst_5', self._rack)
        self.assertIsInstance(i3, pyrack.clInstance)
 
        # Nets
        n1 = nl.findNet('n1top', self._rack)
        self.assertIsInstance(n1, pyrack.clNet)
        n2 = nl.findNet('n2top', self._rack)
        self.assertIsInstance(n2, pyrack.clNet)
        n3 = nl.findNet('n3top', self._rack)
        self.assertIsInstance(n3, pyrack.clNet)
        n4 = nl.findNet('n4top', self._rack)
        self.assertIsInstance(n4, pyrack.clNet)
        n5 = nl.findNet('n5top', self._rack)
        self.assertIsInstance(n5, pyrack.clNet)
        n6 = nl.findNet('n6top', self._rack)
        self.assertIsInstance(n6, pyrack.clNet)
        n7 = nl.findNet('n7top', self._rack)
        self.assertIsInstance(n7, pyrack.clNet)
        n8 = nl.findNet('n8top', self._rack)
        self.assertIsInstance(n8, pyrack.clNet)
 
        # Cross-find: instance.pin
        i1a = i1.findPin('A', self._rack)
        self.assertIsInstance(i1a, pyrack.clPin)
        i1b = i1.findPin('B', self._rack)
        self.assertIsInstance(i1b, pyrack.clPin)
        i1c = i1.findPin('C', self._rack)
        self.assertIsInstance(i1c, pyrack.clPin)
        i1z = i1.findPin('Z', self._rack)
        self.assertIsInstance(i1z, pyrack.clPin)
 
        i2a = i2.findPin('A', self._rack)
        self.assertIsInstance(i2a, pyrack.clPin)
        i2b = i2.findPin('B', self._rack)
        self.assertIsInstance(i2b, pyrack.clPin)
        i2c = i2.findPin('C', self._rack)
        self.assertIsInstance(i2c, pyrack.clPin)
        i2z = i2.findPin('Z', self._rack)
        self.assertIsInstance(i2z, pyrack.clPin)
 
        i3a = i3.findPin('A', self._rack)
        self.assertIsInstance(i3a, pyrack.clPin)
        i3b = i3.findPin('B', self._rack)
        self.assertIsInstance(i3b, pyrack.clPin)
        i3c = i3.findPin('C', self._rack)
        self.assertIsInstance(i3c, pyrack.clPin)
        i3z = i3.findPin('Z', self._rack)
        self.assertIsInstance(i3z, pyrack.clPin)
 
        # Cross-find: net.pin
        # These should be sets instead of pointers due to duplicate names
        # Make sure that the collection can deal with duplicate keys
        n1pina = n1.findPin('A', self._rack)
        self.assertIsInstance(n1pina, pyrack.clPin)
        self.assertEqual(i1a.name().str(), n1pina.name().str())
 
        n1porta = n1.findPort('top_a', self._rack)
        self.assertIsInstance(n1porta, pyrack.clPort)
        self.assertEqual(porta.name().str(), n1porta.name().str())
 
        n2pinb = n2.findPin('B', self._rack)
        self.assertIsInstance(n2pinb, pyrack.clPin)
        self.assertEqual(i1b.name().str(), n2pinb.name().str())
 
        n2portb = n2.findPort('top_b', self._rack)
        self.assertIsInstance(n2portb, pyrack.clPort)
        self.assertEqual(portb.name().str(), n2portb.name().str())
 
        n3pinc = n3.findPin('C', self._rack)
        self.assertIsInstance(n3pinc, pyrack.clPin)
        self.assertEqual(i1c.name().str(), n3pinc.name().str())
 
        n3portc = n3.findPort('top_c', self._rack)
        self.assertIsInstance(n3portc, pyrack.clPort)
        self.assertEqual(portc.name().str(), n3portc.name().str())

        n4pinb = n4.findPins('B', self._rack)
        in4pinb = n4pinb[0]

        n4pinb1 = in4pinb.value()[1]
        print(type(n4pinb1))
#         ASSERT_TRUE(n4pinb1)
#         ASSERT_TRUE(i2b.name().str(), n4pinb1.name().str())
# 
#         n4pinb2 = (++in4pinb).second
#         ASSERT_TRUE(n4pinb2)
#         ASSERT_TRUE(i3b.name().str(), n4pinb2.name().str())
# 
#         ASSERT_TRUE((++in4pinb).name().str(), n4pinb.second)
# 
#         n4portd = n4.findPort('top_d', self._rack)
#         ASSERT_TRUE(n4portd)
#         ASSERT_TRUE(portd.name().str(), n4portd.name().str())
# 
#         n5pinc = n5.findPin('C', self._rack)
#         in5pinc = n5pinc.first
# 
#         n5pinc1 = in5pinc.second
#         ASSERT_TRUE(n5pinc1)
#         ASSERT_TRUE(i2c.name().str(), n5pinc1.name().str())
# 
#         n5pinc2 = (++in5pinc).second
#         ASSERT_TRUE(n5pinc2)
#         ASSERT_TRUE(i3c.name().str(), n5pinc2.name().str())
# 
#         ASSERT_TRUE((++in5pinc).name().str(), n5pinc.second)
# 
#         n5porte = n5.findPort('top_e', self._rack)
#         ASSERT_TRUE(n5porte)
#         ASSERT_TRUE(porte.name().str(), n5porte.name().str())
# 
#         n6pina = n6.findPin('A', self._rack)
#         in6pina = n6pina.first
# 
#         n6pina1 = in6pina.second
#         ASSERT_TRUE(n6pina1)
#         ASSERT_TRUE(i2a.name().str(), n6pina1.name().str())
# 
#         n6pina2 = (++in6pina).second
#         ASSERT_TRUE(n6pina2)
#         ASSERT_TRUE(i3a.name().str(), n6pina2.name().str())
# 
#         ASSERT_TRUE((++in6pina).name().str(), n6pina.second)
# 
#         n6pinz = n6.findPin('Z', self._rack)
#         ASSERT_TRUE(n6pinz)
#         ASSERT_TRUE(i1z.name().str(), n6pinz.name().str())
# 
#         n7pinz = n7.findPin('Z', self._rack)
#         ASSERT_TRUE(n7pinz)
#         ASSERT_TRUE(i2z.name().str(), n7pinz.name().str())
# 
#         n7porty = n7.findPort('top_y', self._rack)
#         ASSERT_TRUE(n7porty)
#         ASSERT_TRUE(porty.name().str(), n7porty.name().str())
# 
#         n8pinz = n8.findPin('Z', self._rack)
#         ASSERT_TRUE(n8pinz)
#         ASSERT_TRUE(i3z.name().str(), n8pinz.name().str())
# 
#         n8portz = n8.findPort('top_z', self._rack)
#         ASSERT_TRUE(n8portz)
#         ASSERT_TRUE(portz.name().str(), n8portz.name().str())
        
    def dissolveTop(self, name):
        print('dissolveTop')

    def verifyFlatTop(self, name):
        print('verifyFlatTop')

        # Base names
#         inst_1 = pyrack.string('inst_1')
#         inst_2 = pyrack.string('inst_2')
#         inst_3 = pyrack.string('inst_3')
#         inst_4 = pyrack.string('inst_4')
#         inst_5 = pyrack.string('inst_5')

        # Hierarchical names
#         im31 = pyrack.clMultiString(inst_3)
#         im31 += inst_1
#         im41 = pyrack.clMultiString(inst_4)
#         im41 += inst_1
#         im51 = pyrack.clMultiString(inst_5)
#         im51 += inst_1
#         im32 = pyrack.clMultiString(inst_3)
#         im32 += inst_2
#         im42 = pyrack.clMultiString(inst_4)
#         im42 += inst_2
#         im52 = pyrack.clMultiString(inst_5)
#         im52 += inst_2
        
        # Instances
#         i31 = nl.findInstance(im31, self._rack)
#         self.assertIsInstance(i31, pyrack.clInstance)
#         i41 = nl.findInstance(im41, self._rack)
#         self.assertIsInstance(i41), pyrack.clInstance
#         i51 = nl.findInstance(im51, self._rack)
#         self.assertIsInstance(i51, pyrack.clInstance)
#         i32 = nl.findInstance(im32, self._rack)
#         self.assertIsInstance(i32, pyrack.clInstance)
#         i42 = nl.findInstance(im42, self._rack)
#         self.assertIsInstance(i42, pyrack.clInstance)
#         i52 = nl.findInstance(im52, self._rack)
#         self.assertIsInstance(i52, pyrack.clInstance)

    _srack = pyrack.string('rack')
    _mrack = pyrack.clMultiString(_srack)
    _rack = pyrack.clRack(_mrack)


class TestRack(unittest.TestCase): 
     
    @classmethod
    def setUpClass(cls):
        cls._sc = SampleDesign()
        cls._sc.setUp()
     
    @classmethod
    def tearDownClass(cls):
        cls._sc.tearDown()
         
    def setUp(self): 
        pass
 
    def test_defaultTechnology(self):
        pass
 
    def test_verifyCell_1(self):
        self.__class__._sc.verifyCell_1('cell_1')

    def test_verifyCell_2(self):
        self.__class__._sc.verifyCell_2('cell_2')

    def test_verifyCell_3(self):
        self.__class__._sc.verifyCell_3('cell_3')

    def test_verifyTop(self):
        self.__class__._sc.verifyTop('RACK')


if __name__ == '__main__': 
    unittest.main() 
