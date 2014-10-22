///(c) 2014, Henrik Reinstädtler
/// von liefert die Rückliegenden Links zurück, start_f_von gibt an, wo ein linkbereich beginnt., status ist die Repräsentation,
// elemente ist die Anzahl der Elemente
//target die Ziel ID--> NULL basiert!
//target status gibt an,  ob ein Ziel erreicht wurde.
__kernel void water_sim(__global int *von, __global int * start_f_von,__global char* status, int elements, int target,char current_round)
{//target_status muss vielleicht zum array werden.
int id = get_global_id(0);
if(id<elements){
if(status[id]==-1)
{
    int maximum_start= (id>=(elements-1))?0:id+1;
    int start_id= (id>=(elements-1))?elements-1:id;
    for(int i=start_f_von[start_id];i<start_f_von[maximum_start];++i)
    {//Achtung moving dependencies durch write--> einsimulationsschritt in mehreren
       if(status[von[i]] == current_round)//vielleicht statt -1 0 nehmen
       {
          //erhöhen
           status[id]=status[von[i]]+1;
           return;//early exit!
       }
    }
}
}
//#schon da
}
