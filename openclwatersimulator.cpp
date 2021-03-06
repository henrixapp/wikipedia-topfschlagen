#include "openclwatersimulator.h"
#include <iostream>
#include <fstream>
#include <xercesc/parsers/SAXParser.hpp>
   #include <xercesc/sax/HandlerBase.hpp>
   #include <xercesc/util/XMLString.hpp>
   #include <iostream>

   using namespace std;
   using namespace xercesc;
OpenCLWaterSimulator::OpenCLWaterSimulator(LinkTree &verb, TreeWikiArticle &arts):articles(arts),verbindungen(verb)
{

    cl::Program program_;
    std::vector<cl::Device> devices;
    try
    {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.size() == 0) {
        std::cerr << "Platform size 0\n";
        throw;
     }
    std::cout<<"Verfügbare Plattformen:"<<platforms.size()<<std::endl;
    int platform=0;
   if(platforms.size()>1){
       int i=0;
    for(auto p=platforms.begin();p!=platforms.end();p++){
        std::cout<<"["<<i++<<"]"<<(*p).getInfo<CL_PLATFORM_NAME>()<<std::endl;
    }
	std::cout<<"Wähle:";
	 std:cin>>platform;
   }
    cl_int err = CL_SUCCESS;
    cl_context_properties properties[] =
       { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[platform])(), 0};
    context = cl::Context(CL_DEVICE_TYPE_ALL, properties);
     devices = context.getInfo<CL_CONTEXT_DEVICES>();
    queue = cl::CommandQueue(context, devices[0], 0, &err);
    for(auto i= devices.begin();i!= devices.end();i++)//Geräteausgeben
    {
        std::cerr<<(*i).getInfo<CL_DEVICE_NAME>()<<std::endl;
    }
    std::ifstream datei("searching.cl");
    std::string loadedSrc((std::istreambuf_iterator<char>(datei)),
                                 std::istreambuf_iterator<char>());
    //Datei geladen nach http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    const char * kernelSource = loadedSrc.c_str();
    cl::Program::Sources source(1,
        std::make_pair(kernelSource,strlen(kernelSource)));
     program_ = cl::Program(context, source);
    program_.build(devices);
   kernel = cl::Kernel(program_, "water_sim", &err);//Kompilieren
   //Größe ändern
   //kernel.getWorkGroupInfo(devices[0],
          // CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE,&preferredSize);
   std::cout<<"Verwendete Größe: "<<preferredSize<<std::endl;
   //Kopieren vorbereiten, erstellen der Daten
   daten = verbindungen.toCL();

   von_cl= cl::Buffer(context,CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR,verbindungen.size()*sizeof(int),daten.first);
   cout<<"Setzen:0"<<endl;
   kernel.setArg(0,von_cl);
   cout<<"erstellen:1"<<endl;
   start_f_von_cl= cl::Buffer(context,CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR,articles.size()*sizeof(int),daten.second);
   cout<<"Setzen:1"<<endl;
}
    catch(cl::Error &e)
    {
        std::cout<<"Ein Fehler ist aufgetreten:"<<e.what()<<"Code:"<<e.err()<<std::endl;
         std::cout<<"LOG:"<<program_.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
        exit(-1);
    }
}
OpenCLWaterSimulator::~OpenCLWaterSimulator()
{
    delete[] daten.first;
    delete[] daten.second;
}


int besuchteFelder(cl_char* status, int length)
{
    int result =0;
    for(int i=0; i<length;i++)
    {
        if(status[i]!= -1) result++;
    }
    return result;
}

std::vector<std::vector<int> >  OpenCLWaterSimulator::suche(int von, int zu)
{
   try{

    kernel.setArg(1,start_f_von_cl);
    cout<<"Working on status..."<<endl;
    //Status array erstellen
    size_t bytes = sizeof(cl_char)*articles.size();
    cl::Buffer status(context, CL_MEM_READ_WRITE, bytes);
    //schreiben dieser Daten
    cl_char *status_array= new cl_char[articles.size()];
    std::fill_n(status_array,articles.size(),(int8_t)-1);
   /* for(int i=0;i<articles.size();i++)
    {
            if(status_array[i]!=-1) cout<<i<<":"<<(int)status_array[i]<<endl;
    }*/
    status_array[von]=(cl_char)0;//zu groß?!
    //int irt;cin>>irt;
    queue.enqueueWriteBuffer(status, CL_TRUE, 0, bytes,status_array);
    //und einen Wert auf null setzen
    kernel.setArg(2,status);
    int elements_count = articles.size();
    cout<<"STATUS set..."<<endl;
    kernel.setArg(3,elements_count);
    cout<<"ElEMCount set..."<<endl;
    kernel.setArg(4,zu);
    cout<<"Target set..."<<endl;
    cl_char status_type=0;
    kernel.setArg(5,(cl_char)0);
    kernel.setArg(6,(cl_int)verbindungen.size());
    //endzustand
    cl::Buffer endstatus(context, CL_MEM_READ_WRITE, sizeof(cl_char));
    cl_char* endstatus_arr=new cl_char[1];
    endstatus_arr[0]=status_type;
    queue.enqueueWriteBuffer(endstatus, CL_TRUE, 0, 1,endstatus_arr);
    kernel.setArg(7,endstatus);
    cout<<"kernel finished..."<<endl;
    cl::NDRange local_n(preferredSize);
    cl::NDRange global_n((int)(ceil(articles.size()/(float)preferredSize)*preferredSize));
    cl_char current_round =0;
    const unsigned long time = XMLPlatformUtils::getCurrentMillis();
    while(status_type==(cl_char)0)
    {

        /*
        int elements = articles.size();
        int * start_f_von =daten.second;
        int * von_array= daten.first;
        int connection_count = verbindungen.size();//This one is new...
        for(int id=0;id<articles.size();id++)
        {
        if(status_array[id]==-1)
        {
            int start_id=start_f_von[id];//later on:     (id>=(elements-1))?elements-1:id;
            int maximum_start= 0;
            if(id<(elements-1))
            {
                maximum_start = start_f_von[id+1];
            } else maximum_start= connection_count;//Rest auslesen...

            for(int i=start_id;i<maximum_start;++i)//hier ischt der Hund begraben--> -1
            {
               if(status_array[von_array[i]] == current_round)//vielleicht statt -1 0 nehmen
               {
                   cout<<"status was different"<<endl;
                  //erhöhen
                   status_array[id]=status_array[von_array[i]]+1;
                   if(id==zu) status_type = status_array[von_array[i]]+1;//hier auch erhöhen
               }
            }
        }
        }
        //nun mal auskellern...
        for(int id=0;id<articles.size();id++)
        {
            cout<<(int)status_array[id]<<endl;
        }
        {int ser;cin>>ser;}
        */
        kernel.setArg(5,current_round);
        cl::Event event,schreiben;
        queue.enqueueNDRangeKernel(kernel,cl::NullRange,global_n,local_n,NULL,&event);
        vector<cl::Event> zuwarten;
        zuwarten.push_back(event);
        char * status_read = (char*)queue.enqueueMapBuffer(endstatus,true,CL_MAP_READ,0,1,&zuwarten,&schreiben);
        schreiben.wait();
        //  cout<<"Erfolgreich gemappt"<<endl;
        status_type =status_read[0];
        cl::Event unmapping;
        queue.enqueueUnmapMemObject(endstatus,status_read,NULL,&unmapping);
        unmapping.wait();
       // cout<<"Status was "<<(int)(status_type)<<endl;
        current_round++;//erhöhen
    }

    cl::Event lesen;
    cl_char * status_read =(cl_char*)queue.enqueueMapBuffer(status,true,CL_MAP_READ,0,articles.size(),NULL,&lesen);
    lesen.wait();
    const unsigned long time2 = XMLPlatformUtils::getCurrentMillis();
    cout<<"Wegfindung nach "<<time2-time<<" Millisekunden abgeschlossen, erwartetes Ergebnis:"<<(int)status_read[zu]<<endl;
    cout<<"Besuchte Felder:"<<besuchteFelder(status_read,articles.size())<<endl;
    auto result = tools::traceBack<cl_char>(daten.first,daten.second,status_read,von,zu,articles.size(),verbindungen.size(),[=](int id){
        return articles.find(id)->Titel();
    });
    cl::Event unmapping;
    queue.enqueueUnmapMemObject(status,status_read,NULL,&unmapping);
    unmapping.wait();
    delete[] status_array;
    return result;
    }
    catch(cl::Error &e)
    {
        cout<<"Fehler:"<<e.err()<<" bei "<<e.what()<<endl;
    }
}
/**
 * @brief OpenCLWaterSimulator::parameterisierteAusfuehrung Führt die Simulation mit Hilfe des Kernels durch.
 * Es gibt eine Einschränkung von den Stellen, dann ist aber auch wirklich alles erlaub ;)
 * weiter_laufen: READ only!!!...
 * parameter, aktuelle Runde, Zeit...
 */
void OpenCLWaterSimulator::parameterisierteAusfuehrung(function<bool(char*,unsigned long,unsigned long)> weiter_laufen,function<void(char*)> initialisierung,
                                                       function<void(char*)> endbewertung)
{
    try{
     kernel.setArg(1,start_f_von_cl);
     cout<<"Working on status..."<<endl;
     //Status array erstellen
     size_t bytes = sizeof(cl_char)*articles.size();
     cl::Buffer status(context, CL_MEM_READ_WRITE, bytes);
     //schreiben dieser Daten
     cl_char *status_array= new cl_char[articles.size()];
     std::fill_n(status_array,articles.size(),(int8_t)-1);
    initialisierung((char*)status_array);
     //should be change to mapping... write only--- memory save
     queue.enqueueWriteBuffer(status, CL_TRUE, 0, bytes,status_array);
     //und einen Wert auf null setzen
     kernel.setArg(2,status);
     int elements_count = articles.size();
     kernel.setArg(3,elements_count);
     kernel.setArg(4,0);
     //kernel.setArg(4,zu);
     cl_char status_type=-1;
     kernel.setArg(5,(cl_char)0);
     kernel.setArg(6,(cl_int)verbindungen.size());
     cl::NDRange local_n(preferredSize);
     cl::NDRange global_n((int)(ceil(articles.size()/(float)preferredSize)*preferredSize));
     cl_char current_round =0;
     const unsigned long time = XMLPlatformUtils::getCurrentMillis();
     bool running= weiter_laufen((char*)status_array,current_round,0);//erstes Aufsetzen...
     //schonmal weg gehen

     while(running)
     {
         kernel.setArg(5,current_round);
         cl::Event event,schreiben;
         queue.enqueueNDRangeKernel(kernel,cl::NullRange,global_n,local_n,NULL,&event);
         vector<cl::Event> zuwarten;
         zuwarten.push_back(event);
         char * status_read = (char*)queue.enqueueMapBuffer(status,true,CL_MAP_READ,0,articles.size(),&zuwarten,&schreiben);
         schreiben.wait();
      //   cout<<"Erfolgreich gemappt"<<endl;
         running = weiter_laufen((char*)status_read,current_round,XMLPlatformUtils::getCurrentMillis()-time);
         cl::Event unmapping;
         queue.enqueueUnmapMemObject(status,status_read,NULL,&unmapping);
         unmapping.wait();
         current_round++;//erhöhen
     }

     cl::Event lesen;
     cl_char * status_read =(cl_char*)queue.enqueueMapBuffer(status,true,CL_MAP_READ,0,articles.size(),NULL,&lesen);
     lesen.wait();
     const unsigned long time2 = XMLPlatformUtils::getCurrentMillis();
     cout<<"Wegfindung nach "<<time2-time<<" Millisekunden abgeschlossen";
     cout<<"Besuchte Felder:"<<besuchteFelder(status_read,articles.size())<<endl;
     endbewertung((char*)status_read);
     cl::Event unmapping;
     queue.enqueueUnmapMemObject(status,status_read,NULL,&unmapping);
     unmapping.wait();
     delete[] status_array;
     }
     catch(cl::Error &e)
     {
         cout<<"Fehler:"<<e.err()<<" bei "<<e.what()<<endl;
     }
}
