**Fluffy Teeworlds**
This client uses a custom base class that you can use to make hacks. 
See game/client/components/snaffhacks for examples.
For a class to actually execute you need to reference it in gameclient.cpp for example: ```m_Hacks.push_back(new HAimbot(this));```. It works like the component.h, but it gots extra functionality to make hacks easier.
