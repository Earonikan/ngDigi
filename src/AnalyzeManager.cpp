#include "AnalyzeManager.h"

AnalyzeManager::AnalyzeManager()
{
    ;
}

void AnalyzeManager::CreateHistos(Aconfig aconfig, Dconfig dconfig, CAEN_DGTZ_BoardInfo_t BoardInfo)
{
	aconfig_ = aconfig;
	dconfig_ = dconfig;
	BoardInfo_ = BoardInfo;

	char sname[200], stitle[200];
	for (auto i = 0; i < int(dconfig_.chtype_db.size()); i++)
	{
		if (dconfig_.chtype_db[i] == 1) 
		{	
			histocollection_.signal_ch.push_back(i);
			sprintf(sname, "signalwf_%d", i);
			sprintf(stitle, "signal_waveform_trigger_ch%d", i);
			if (histocollection_.Wfm_signal_1[i]) histocollection_.Wfm_signal_1[i]->Delete();
			histocollection_.Wfm_signal_1[i] = new TH1F(sname, stitle, dconfig_.Samples, 0, dconfig_.Samples);
			if (strcmp("DT5720", BoardInfo_.ModelName) == 0) histocollection_.Wfm_signal_1[i]->GetXaxis()->SetTitle("t, 4ns");
			if (strcmp("DT5730", BoardInfo_.ModelName) == 0) histocollection_.Wfm_signal_1[i]->GetXaxis()->SetTitle("t, 2ns");
		}

		if (dconfig_.chtype_db[i] == 2) 
		{
			histocollection_.trigger_ch.push_back(i);
			sprintf(sname, "trigwf_%d", i);
			sprintf(stitle, "trigger_waveform_ch%d", i);
			if (histocollection_.Wfm_trigger[i]) histocollection_.Wfm_trigger[i]->Delete();
			histocollection_.Wfm_trigger[i] = new TH1F(sname, stitle, dconfig_.Samples, 0, dconfig_.Samples);
			if (strcmp("DT5720", BoardInfo_.ModelName) == 0) histocollection_.Wfm_trigger[i]->GetXaxis()->SetTitle("t, 4ns");
			if (strcmp("DT5730", BoardInfo_.ModelName) == 0) histocollection_.Wfm_trigger[i]->GetXaxis()->SetTitle("t, 2ns");
		}

		// std::cout << std::endl;

		// sprintf(sname,"signaldata_%d", i);
		// sprintf(stitle,"signal_data_trigger_ch%d",chtr[i]);
		// if (hsdata[i]) hsdata[i]->Delete();
		// hsdata[i]=new TH1F(sname,stitle,samples,0,samples);


		// sprintf(sname,"acsignalwf_%d",chtr[i]);
		// sprintf(stitle,"accum_signal_waveform_trigger_ch%d",chtr[i]);
		// if (hacsignal[i]) hacsignal[i]->Delete();
		// hacsignal[i]=new TH1F(sname,stitle,samples,0,samples);
		// hacsignal[i]->GetXaxis()->SetTitle("4ns");
		// sprintf(sname,"signal_spectr_%d",chtr[i]);
		// sprintf(stitle,"signal_spectrum_trigger_ch%d",chtr[i]);
		// if (hsigspectr[i]) hsigspectr[i]->Delete();
		// hsigspectr[i]=new TH1F(sname,stitle,1000,rmin,rmax);
		// hsigspectr[i]->SetLineColor(5+i);
		// sprintf(sname,"bl_spectr_%d",chtr[i]);
		// sprintf(stitle,"baseline_spectrum_trigger_ch%d",chtr[i]);
		// if (hblspectr[i]) hblspectr[i]->Delete();
		// hblspectr[i]=new TH1F(sname,stitle,1000,rmin,rmax);
		// hblspectr[i]->SetLineColor(3+i);
		// sprintf(sname,"common_spectr_%d",chtr[i]);
		// sprintf(stitle,"common_spectrum_trigger_ch%d",chtr[i]);
		// if (hcommon[i]) hcommon[i]->Delete();
		// //   hcommon[i]=new TH1F(sname,stitle,(rmax-rmin+300)/(4+(1-i)*4),-300,(rmax-rmin));
		// if (i==0) hcommon[i]=new TH1F(sname,stitle,(rmax-rmin+1000),-1000,(rmax-rmin));
		// if (i!=0) hcommon[i]=new TH1F(sname,stitle,(rmax-rmin+1000)/2,-1000,(rmax-rmin));
		// if (tpoly[i]) tpoly[i]->Delete();
		// tpoly[i]=new TBox(intsig[i],0,intsig[i]+windowwidth,17000);
		// tpoly[i]->SetFillColorAlpha(kRed,0.8);
		
		// sprintf(sname,"pave_%d",chtr[i]);

		/*	   if(!tpave[i]) {
		tpave[i]= new TPaveText();
		tpave[i]->SetName(sname);
		//tpave[i]->SetTextSize(0.025);
		//tpave[i]->SetTextAlign(13);  //align at top
		//tpave[i]->DrawLatex(.2,.9,"K_{S}");
		//tpave[i]->DrawLatex(.3,.9,"K^{*0}");
		tpave[i]->SetX1NDC(0.);
		tpave[i]->SetX2NDC(1.);
		tpave[i]->SetY1NDC(0);
		tpave[i]->SetY2NDC(1.);
		tpave[i]->SetOption("");
		tpave[i]->SetFillColorAlpha(kRed,0.8);
		//if(hcommon[i])hcommon[i]->GetListOfFunctions()->Add(tpave[i]);
		
		//	tpave[i]->AddText("test");
		}*/


	}

	// if (htrev) htrev->Delete();
	// htrev=new TH1F("trev","trev",4,-1,3);
	// if (hminimum0) hminimum0->Delete();
	// hminimum0=new TH1F("minimum0","minimum0",16000,0,16000);
	// if (hminimum2) hminimum2->Delete();
	// hminimum2=new TH1F("minimum2","minimum2",16000,0,16000);
	// if (hminimumsig) hminimumsig->Delete();
	// hminimumsig=new TH1F("minimumsig","minimumsig",16000,0,16000);
 
	
}

void AnalyzeManager::DeleteHistos()
{
	for (auto i = histocollection_.Wfm_trigger.cbegin(), end = histocollection_.Wfm_trigger.cend(); i != end; i++)
	{
		delete i->second;
	}
	for (auto i = histocollection_.Wfm_signal_1.cbegin(), end = histocollection_.Wfm_signal_1.cend(); i != end; i++)
	{
		delete i->second;
	}
}

void AnalyzeManager::AnalyzeData()
{
//     for(int j = 0; j < (int)NEvents; j++) 
//      {
     

//       /* Get one event from the readout buffer */
//       ret = CAEN_DGTZ_GetEventInfo(handle, buffer, BufferSize, j, &EventInfo, &EventPtr);
//       //cout<<ret<<endl;
//       if (ret) {
// 	cout<<"GetEventInfo "<<NEvents<<" "<<j<<" "<<errors[abs((int)ret)]<<" (code "<<ret<<")"<<endl; return 0;
// 	//	continue;
// 	//exit(0);
//       }
//       //printf("Decode\n");
//       /* decode the event */
//       ret = CAEN_DGTZ_DecodeEvent(handle, EventPtr, (void**)&Event16);

//       if (ret) {
// 	cout<<errors[abs((int)ret)]<<" (code "<<ret<<")"<<endl;
// 	exit(0);
//       }


   
//       int trev=-1; int bl_=0,sig_=0,sumdata=0;int minimumsig=0;
//      int maximumvalue[2]={0,0};
//      if (trtype==0)
//        {
//      for(int i=int(samples*(100-posttrigger)/100.); i>5; i--){
//        if ((maximumvalue[0]==0)&&(Event16->DataChannel[chtr[0]][i]>int(thresh_db[chtr[0]]))) maximumvalue[0]=1;
//        if ((maximumvalue[1]==0)&&(Event16->DataChannel[chtr[1]][i]>int(thresh_db[chtr[1]]))) maximumvalue[1]=1;
//      }
//    for(int i=int(samples*(100-posttrigger)/100.); i>5; i--)
//      {
//          //if((int(-Event16->DataChannel[chtr[1]][i])+int(Event16->DataChannel[chtr[1]][i-5]))>int(.2*thresh_db[chtr[1]])) {trev=1;break;}
//        if ((maximumvalue[0]==1)&&(int(Event16->DataChannel[chtr[0]][i])<int(thresh_db[chtr[0]]))) {trev=0;break;}
//        if ((maximumvalue[1]==1)&&(int(Event16->DataChannel[chtr[1]][i])<int(thresh_db[chtr[1]]))) {trev=1;break;}
// 	 /*  if((int(-Event16->DataChannel[chtr[0]][i])+int(Event16->DataChannel[chtr[0]][i-5]))>int(.2*thresh_db[chtr[0]])) {trev=0;break;}*/
       
//      }
//        }
//      if (trtype==1) trev=0;
//    if (trev!=-1)
//      {
//         int minvalue0=16000; int minvalue2=16000;
//        int maxvalue0=0; int maxvalue2=0;
//        int sam0=0,sam2=0;
//        int q0=0,q2=0;
//        int bls0=0;int bls2=0;
//        for(int i=int(samples*(100-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.); i--) {q0+=int(Event16->DataChannel[chtr[0]][i]);sam0++;if (minvalue0>int(Event16->DataChannel[chtr[0]][i])) minvalue0=int(Event16->DataChannel[chtr[0]][i]);}
//        for(int i=int(samples*(100-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.); i--) {q2+=int(Event16->DataChannel[chtr[1]][i]);sam2++;if (minvalue2>int(Event16->DataChannel[chtr[1]][i])) minvalue2=int(Event16->DataChannel[chtr[1]][i]);}
//        for(int i=int(samples*(100-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.); i--) if (maxvalue0<int(Event16->DataChannel[chtr[0]][i])) maxvalue0=int(Event16->DataChannel[chtr[0]][i]);
//        for(int i=int(samples*(100-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.); i--) if (maxvalue2<int(Event16->DataChannel[chtr[1]][i])) maxvalue2=int(Event16->DataChannel[chtr[1]][i]);
//        for(int i=int(samples*(90-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.)-10; i--)  bls0+=int(Event16->DataChannel[chtr[0]][i]);
//        for(int i=int(samples*(90-posttrigger)/100.); i>int(samples*(90-posttrigger)/100.)-10; i--)  bls2+=int(Event16->DataChannel[chtr[1]][i]);		      
//        hminimum0->Fill(TMath::Abs(bls0/10.-maxvalue0));//printf("%d %d\n",bls0,minvalue0);
//        hminimum2->Fill(TMath::Abs(bls2/10.-maxvalue2));//printf("%d %d\n",bls2,minvalue2);
//        if (TMath::Abs(bls0/10.-minvalue0)>20+0*TMath::Abs(bls0/10.-chtr[0])) {trev=-1;printf("Noize ch0 q=%d bl=%d max=%d min=%d\n",int(q0/sam0),int(bls0/10.),maxvalue0,minvalue0);}
//        if (TMath::Abs(bls2/10.-minvalue2)>20+0*TMath::Abs(bls2/10.-chtr[1])) {trev=-1;printf("Noize ch2 q=%d bl=%d max=%d min=%d\n",int(q2/sam2),int(bls2/10.),maxvalue2,minvalue2);}
//      }

//    //trev=1;
//    uint16_t sdata[samples];
//     for (int ui=0;ui<samples;ui++) sdata[ui]= 16000-Event16->DataChannel[chsig][ui];
//    //  for (int ui=0;ui<samples;ui++) sdata[ui]= Event16->DataChannel[chsig][ui]; 
//    for(int ch=0; ch<NUMBER_OF_CHANNELS; ch++){

//      if (EnableMask & (1<<ch)){
       
//        int Size=Event16->ChSize[ch];
//        if(Size<=0)
// 	 continue;
    
//        for(int i=0; i<Size; i++){
//         if (ch==chtr[0]) htrigger[0]->SetBinContent(i+1,Event16->DataChannel[ch][i]);
// 	if (ch==chtr[1]) htrigger[1]->SetBinContent(i+1,Event16->DataChannel[ch][i]);

// 	if((ch==chsig)&&(trev!=-1)){
// 	  /*
// 	hsignal[trev]->SetBinContent(i+1,Event16->DataChannel[ch][i]);
// 	if (i>4&&i<1000)
// 	  {
// 	    sumdata=Event16->DataChannel[ch][i]*8+Event16->DataChannel[ch][i-1]*4+Event16->DataChannel[ch][i+1]*4+Event16->DataChannel[ch][i-2]*2+Event16->DataChannel[ch][i+2]*2+Event16->DataChannel[ch][i-3]+Event16->DataChannel[ch][i-3];
// 	    sumdata=(Event16->DataChannel[ch][i]+Event16->DataChannel[ch][i-1]+Event16->DataChannel[ch][i+1]+Event16->DataChannel[ch][i-2]+Event16->DataChannel[ch][i+2]+Event16->DataChannel[ch][i-3]+Event16->DataChannel[ch][i+3]);
// 	    Double_t Mmin=1000;
// 	    for(int mini=-3;mini<4;mini++){Mmin=TMath::Min(Mmin,TMath::Abs(Event16->DataChannel[ch][i+mini]-sumdata/7.0));}
// 	    hsdata[trev]->SetBinContent(i,(sumdata/7.0*(Mmin<1))+Event16->DataChannel[ch][i]*(Mmin>=1));
// 	  }

// 	if((i>intbl[trev])&&(i<intbl[trev]+windowwidth)) bl_+=Event16->DataChannel[ch][i];
// 	if((i>intsig[trev])&&(i<intsig[trev]+windowwidth)) sig_+=Event16->DataChannel[ch][i];*/
// 	hsignal[trev]->SetBinContent(i+1,sdata[i]);
// 	if((i>intbl[trev])&&(i<intbl[trev]+windowwidth)) bl_+=sdata[i];
// 	if((i>intsig[trev])&&(i<intsig[trev]+windowwidth)){
// 	  sig_+=sdata[i];
// 	  if (minimumsig<sdata[i]) minimumsig=sdata[i];
// 	}

// 	}
//        }
//        //cout<<ch<<endl;
//      }
//    }
//    if (trev!=-1) 
//      {
//        //TSpectrum *s = new TSpectrum(2*1);
//        //hsdata[trev]=s->Background(hsignal[trev],20,"same");
//        hacsignal[trev]->Add(hsignal[trev]);
//        hblspectr[trev]->Fill(bl_);
//        hsigspectr[trev]->Fill(sig_);
//        hcommon[trev]->Fill((sig_-bl_));
//        hminimumsig->Fill(minimumsig-(1.*bl_)/(windowwidth-1));
       
//      }
//    htrev->Fill(trev);
//      }
//    return 1;
}
