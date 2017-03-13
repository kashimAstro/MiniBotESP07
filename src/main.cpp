#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxNetwork.h"

/* #define HTTP */ 
#define PORT 9055 /* TCPServer example for ESP */

class ofApp : public ofBaseApp
{
	public:
		#ifdef HTTP
			ofURLFileLoader http;
		#else
			ofxTCPClient tcp;
		#endif
		string url,mode;

		ofApp(string _u, string _m)
		{
			url  = _u;
			mode = _m;
		}

		void setup()
		{
			#ifdef HTTP
				ofHttpResponse r = ofLoadURL("http://"+url+"/"+mode);
				cout << r.data << endl;
			#else
				tcp.setup(url,PORT);
			        tcp.setMessageDelimiter("\n");
				if(tcp.isConnected())
				{
				        tcp.send(mode);
				        while( 1 )
					{
	        				string str = tcp.receive();
						if(str.length() > 0)
						{
							tcp.close();
				                        ofLog()<< str;
							abort();
							ofExit(0);	
						}
						ofSleepMillis(1);
				        }
				}
			#endif
			ofExit(0);
		}
};

int main(int argc, char *argv[])
{
	ofAppNoWindow w;
	ofSetupOpenGL(&w,0,0, OF_WINDOW);
	if(argc > 2) 
	{
		ofRunApp( new ofApp(argv[1], argv[2]) );
	}
	else
	{
		cout<<"Parameter: [ address ] [ pin ]\n";
		cout<<"\tExample: ./bin/esp-07 192.168.1.10 gpio13\n";
		ofExit(0);
	}
}
