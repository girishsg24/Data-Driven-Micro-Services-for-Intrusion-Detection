source::FromDevice;                                                                                
sink::ToDevice;       

c::Classifier(12/0800 34/08,-);  
source->c;                

//************************Important piece********************                                                  
c[0]->Print("Before",MAXLENGTH -1)
// ->Strip(14)->StripIPHeader->Print("Strip",MAXLENGTH -1)
 ->CheckWaterMark
// ->UnstripIPHeader->Unstrip(14)
->Print("After",MAXLENGTH -1)->sink;
//*************************************************************

c[1]->sink; 
