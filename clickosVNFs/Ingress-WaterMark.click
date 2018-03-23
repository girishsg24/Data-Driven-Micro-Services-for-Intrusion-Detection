define($IP 10.10.3.7);                                                                            
define($MAC 00:00:00:00:30:00);   

source::FromDevice;                                                                                
sink::ToDevice;       

c::Classifier(12/0806,12/0800 34/08,-);  

source->c;      

c[0]->ARPResponder($IP $MAC)->sink;                      
//************************Important piece********************                                                  
c[1]->CheckIPHeader(14)->Print("Before",MAXLENGTH -1)
 ->Strip(14)->StripIPHeader
 ->SetWaterMark
 ->UnstripIPHeader->Unstrip(14)->Print("After",MAXLENGTH -1)->sink;
//*************************************************************
c[2]->sink; 
