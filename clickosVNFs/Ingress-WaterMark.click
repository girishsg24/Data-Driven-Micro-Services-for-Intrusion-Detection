source::FromDevice;                                                                                
sink::ToDevice;       

c::Classifier(12/0800,-);  

source->c;      
                    
//************************Important piece********************                                                  
c[0]->Print("Before",MAXLENGTH -1)
 ->CheckIPHeader(14)
 ->Strip(14)
 ->StripIPHeader
 ->SetWaterMark
 ->UnstripIPHeder
 ->Unstrip(14)
 ->CheckIPHeader(14)
 ->Print("After",MAXLENGTH -1)->sink;
//*************************************************************
c[1]->sink; 
