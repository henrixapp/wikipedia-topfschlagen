///(c) 2014, Henrik Reinstädtler
/// von liefert die Rückliegenden Links zurück, start_f_von gibt an, wo ein linkbereich beginnt., status ist die Repräsentation,
// elemente ist die Anzahl der Elemente
//target die Ziel ID--> NULL basiert!
//target status gibt an,  ob ein Ziel erreicht wurde.
__kernel void water_sim(__global int *von_array, __global int * start_f_von,__global char* status_array, int elements, int target,char current_round, int connection_count)
{//target_status muss vielleicht zum array werden.
int id = get_global_id(0);
if(id<elements){
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
          //erhöhen
           status_array[id]=status_array[von_array[i]]+1;
           return;//Early exiting...
       }
    }
}
}
//#schon da
}
