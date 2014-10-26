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
    cl_int err = CL_SUCCESS;
    cl_context_properties properties[] =
       { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(), 0};
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
   daten = verbindungen.toCL(articles);

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

//multi dimensional, da es mehrere Möglichkeiten gibt.
//zunächst aber ein dimensional..., wobei wenn wir immer die teilstücke nehmen und zusammenstecken, abhängig von der Position
//durch wechsel des ZU articles...
//Der erste artikel wird !immer hinzugefügt!
std::vector<std::vector<int> > traceBack(int* von, int* start_f_von, cl_char* status,int von_article , int zu_article, int size, int size_connections,function<string(int)> converter)
{
    //Backbouncing
    std::vector<std::vector<int> >  result;
    vector<int> ergebnis;
   // cin>>von_article;
    int article=zu_article;

    if(article!=von_article)
    {
      //  cout<<"article"<<article<<endl;
        cl_char status_article = status[article];
       // cout<<"status is"<<(int)status_article<<endl;
        int maximum = ((article+1)<size)?start_f_von[article+1]:size_connections;
//        cout<<"M"<<maximum<<endl;
        for(int j=start_f_von[article];j<maximum;j++)//TODO: bound check...
        {
            if(status[von[j]]==status_article-1)
            {
              //  cout<<"Exchangign"<<von[j]<<converter(von[j])<<endl;
                //Rekur
                auto zw = traceBack(von,start_f_von,status,von_article,von[j],size,size_connections,converter);
                for(auto i=zw.begin();i!=zw.end();++i)
		{
			result.push_back(*i);
		}
		//article= von[j];//Fixed--> should make translation
                //cin>>j;
            }
        }
    }
    else {
	//ergebnis.push_back(article);
	result.push_back(ergebnis);//empty end recursion
}
    for(auto i = result.begin();i!=result.end();++i)
    {
	(*i).push_back(article);
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
    cl_char status_type=-1;
    kernel.setArg(5,(cl_char)0);
    kernel.setArg(6,(cl_int)verbindungen.size());
    cout<<"kernel finished..."<<endl;
    cl::NDRange local_n(preferredSize);
    cl::NDRange global_n((int)(ceil(articles.size()/(float)preferredSize)*preferredSize));
    cl_char current_round =0;
    const unsigned long time = XMLPlatformUtils::getCurrentMillis();
    while(status_type==(cl_char)-1)
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
        char * status_read = (char*)queue.enqueueMapBuffer(status,true,CL_MAP_READ,0,articles.size(),&zuwarten,&schreiben);
        schreiben.wait();
     //   cout<<"Erfolgreich gemappt"<<endl;
        status_type =status_read[zu];
        cl::Event unmapping;
        queue.enqueueUnmapMemObject(status,status_read,NULL,&unmapping);
        unmapping.wait();
       // cout<<"Status was "<<(int)(status_type)<<endl;
        current_round++;//erhöhen
    }

    cl::Event lesen;
    cl_char * status_read =(cl_char*)queue.enqueueMapBuffer(status,true,CL_MAP_READ,0,articles.size(),NULL,&lesen);
    lesen.wait();
    const unsigned long time2 = XMLPlatformUtils::getCurrentMillis();
    cout<<"Wegfindung nach "<<time2-time<<" Millisekunden abgeschlossen, erwartetes Ergebnis:"<<(int)status_read[zu]<<endl;
    auto result = traceBack(daten.first,daten.second,status_read,von,zu,articles.size(),verbindungen.size(),[=](int id){
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
