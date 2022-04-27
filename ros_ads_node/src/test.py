import pyads

# connect to plc and open connection
plc = pyads.Connection('172.18.236.170.1.1', 851, '172.21.176.50')
plc.open()

plc.read_device_info()

plc.read_state()
print(plc.get_all_symbols())

# read int value by name
i = plc.read_by_name("GVL_ROS.TOTO")
print(i)
# write int value by name
plc.write_by_name("GVL_ROS.TOTO", 2)
print("closing")
# close connection
plc.close()