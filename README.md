Getting Started

Run the script register.sh as follows
./register.sh

To enumerate class instance
--------------------------

wbemcli ei http://localhost:5988/root/cimv2:GK_est



Indication
----------
#Indication test files are available at test/indication folder.

# (SFCB)
# ------

# SFCB allows indications to be delivered directly to a text file. You simply need
# to load the sample filter, handler, and subscription by:

# wbemcat SFCBCreateFilter.localhost.xml
# wbemcat SFCBCreateHandler.localhost.xml
# wbemcat SFCBCreateSubscription.localhost.xml

# By default the sample handler sets "/tmp/SFCB_OS_Listener.txt" as the file to
# deliver indications. You can changes this destination by editing the "Destination"
# property in the handler creation XML.

#Unload via the following order:

# wbemcat SFCBDeleteSubscription.localhost.xml
# wbemcat SFCBDeleteHandler.localhost.xml
# wbemcat SFCBDeleteFilter.localhost.xml

