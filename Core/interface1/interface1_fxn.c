///*
// * interface1_fxn.c
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
//
//#include "interface1.h"
//#include "../interface1/interface_fxn1.h"
//#include "config.h"
//#include "pump_comm.h"
//
//extern int server_time;
//
//extern config_struct config_data;
//extern pump_   pump[2];
//
//bool authenticated = 0;
//uint8_t pump_no =  0;
//float amt_ = 0.0;
//
//int val;
//char conv_out[20] = {0};
//
//char attendant_[20] = {0};
//char attendant_2[20] = {0};
//
//rcv_struct  msg_str;
//extern msgg  retMsg[2] , mainMessage;
//extern char my_message[400];
//
//bool serv_message_received = 0;
//
////================================
//char tag[18] = {0};
//char transaction_type[2] = {0};     // for side 1.
//
//char tag2[18] = {0};
//char transaction_type2[2] = {0};    // for side 2.
////================================
//
//uint8_t server_stat = 0;
//
//ep4_callback cb[2] , cb2 ;  //EP4 message callbacks...
//
//
//void status(int inx)
//{
//    //get pump status..
//
//	//pump_state_enum
//	uint8_t p1_state =  pump[0].state;
//	//pump_state_enum
//	uint8_t p2_state =  pump[1].state;
//
//	uint8_t mode1 = pump[0].mode;
//	uint8_t mode2 = pump[1].mode;
//
//	float price_pl1 = (pump[0].unitprice > 0 ? pump[0].unitprice:config_data.pump[0].price);
//	float price_pl2 = (pump[1].unitprice > 0 ? pump[1].unitprice  :config_data.pump[1].price);
//
//	float vol_total1 = pump[0].vol_totalizer;
//	float vol_total2 = pump[1].vol_totalizer;
//
//	float vol_transaction1 = pump[0].filled_vol;
//	float vol_transaction2 = pump[1].filled_vol;
////===================================================================================
////check if AUTO activity is onging...
//	//int indx_p = 0;
//	if(server_stat != 0)
//	{
////	    DEBUG_FLASH("stored pump name: %d ",cb.pump_name);
//	    if ( strcmp( pump[0].name,(cb[inx].pump_name) ) != 0 )
//		{
//			//DEBUG_FLASH("Server status not 0 ,pump status for pump[0]:  p1 : %d  p2   : %d",p1_state,p2_state);
//	    	p1_state = server_stat;
//		}
//
//	    if ( strcmp( pump[1].name,(cb[inx].pump_name) ) != 0 )
//			{
//				//DEBUG_FLASH("Server status not 0 ,pump status for pump[0]:  p1 : %d  p2   : %d",p1_state,p2_state);
//				p2_state = server_stat;
//			}
//		// server has a pending state...
//
//	      if(server_stat != server_busy)
//			{
//				//clear flag only when not busy
//	    	   server_stat = 0;
//			}
//	}
////    DEBUG_FLASH("pump status p1 : %d  p2   : %d",p1_state,p2_state);
////    DEBUG_FLASH("pump per litre p1 : %f  p2   : %f",price_pl1,price_pl2);
////    DEBUG_FLASH("vol total vt1: %f  vt2: %f",vol_total1,vol_total2);
////    DEBUG_FLASH("volume transaction v1: %f  v2: %f",vol_transaction1,vol_transaction2);
//
////	    string stat1  = "";
////	    string stat2  = "";
//
////	    stringstream str;
////	    str << p1_state;
////	    str >> stat1;
//
//	char stat1,stat2 = {0};
//
//	 sprintf(stat1,"%d",p1_state);          //    _int2str(p1_state);   stat1 = conv_out;
//	 sprintf((char*)stat2,"%d",p2_state);   //    _int2str(p2_state);   stat2 = conv_out;
//
//	 memset(msg_str.head,0,sizeof(msg_str));
//
//		                msg_str.head = '[';
//			     	 	msg_str.tail = ']';
//			     	 	msg_str.cmd[0] = '1'; //== stop command is 04.
//			     	 	msg_str.cmd[1] = '2'; //
//			     	 	msg_str.cmd[2] = 0;   //   terminate string.
//			     	  memcpy(msg_str.id, mainMessage.id,strlen(mainMessage.id));
//			     	//   msg_str.msg = msg;
//
//  sprintf(msg_str.msg,"%s:%s:%2f:%2f:%2f,%c|%s:%s:%2f:%2f:%2f,%c",
//		  config_data.pump[0].name,stat1,price_pl1,vol_total1,vol_transaction1,mode1,
//		  config_data.pump[1].name,stat2,price_pl2,vol_total2,vol_transaction2,mode2);
//
//				   msg_str.ln_ = strlen(msg_str.msg);
//
//			 	  msg_str.msg[ msg_str.ln_] = 0;
//			 	  msg_str.msg[ msg_str.ln_++] = 0;  //null termination.
//
//					BuildString();
//
//			      uartSendData( my_message );
//}
////========================================================================
//void auth_amount(int nn)
//{
//int jmp= 0 ;
////	string pump_name = "";
////	string token_ = "";
////	string amount = "";
//
//	char pump_name[3] = {0};
//	char token_[12]   = {0};
//	char amount[9] 	  = {0};
//
//	uint8_t indx = 0;
//
//	int pump_indx = 0;
//	 uint8_t ack_stat = 0;
//
//
//
//
//		while ( mainMessage.msg[indx] != ':' )
//		{
//			pump_name[indx] = mainMessage.msg[indx];
//			indx++;
//		}
//		pump_name[indx] = 0;
//		// DEBUG_FLASH("pump name = %s",pump_name);
//
//
//	//-------------------------------------------------------
//	// get pump's index...
//		 pump_indx = get_pump_index_from_name_string(pump_name);
//			//	DEBUG_FLASH("[%s] name %s is index: %d", __FUNCTION__,pump_name, pump_indx);
//
//		if (pump_indx < 0)
//		{
//			//DEBUG_FLASH("[%s] name %s not found", __FUNCTION__,pump_name);
//			ack_stat = failed;
//			goto exit_auth_amount;     //send failure message..
//		}
//	//--------------------------------------------------------------
//		if(pump_indx == 0)
//		{
//		  	//log off attendant if any authorise volume or price is sent
//
//		  		memset(config_data.others.atag,0,20);    // clear the attendant's credentials from memory.
//
//		  		//del_tag();       //clear the tag from non volatile memory.
//		  		memset(attendant_,0,20);
//		  		//change the pump back to auto mode.......
//		  		//-----------------------------------------------
//				//---    set both sides to AUTO MODE.
////						 pump[0]->setMode(AUTO);
////						 pump[1]->setMode(AUTO);
//
//						 write_att_tag(clear);
//
//		  	//--------------------------------------------------------------
//		}
//		else
//		{
//			//log off attendant if any authorise volume or price is sent
//
//			memset(config_data.others.atag2,0,20);  //config_data.others.atag2 = "";   // clear the attendant's credentials from memory.
//
//				  		//del_tag();       //clear the tag from non volatile memory.
//				  		memset(attendant_2,0,20);
//				  		//change the pump back to auto mode.......
//				  		//-----------------------------------------------
//						//---    set both sides to AUTO MODE.
//		//						 pump[0]->setMode(AUTO);
//		//						 pump[1]->setMode(AUTO);
//
//								 write_att_tag2(clear);
//
//				  	//--------------------------------------------------------------
//		 }
//
//
////---------------------------------------------------------------
//	//look for transaction type
//	 indx++;          //@  transaction type
////---------------------------------------------------------------
//   if (pump_indx == 0 )
//   {
//	 transaction_type[0] = mainMessage.msg[indx];
//	 transaction_type[1] = 0;  //terminate string.
//	// DEBUG_FLASH("transaction 1 type = %s",transaction_type);
//   }
//   else
//   {
//	 transaction_type2[0] = mainMessage.msg[indx];
//	 transaction_type2[1] = 0;  //terminate string.
//	// DEBUG_FLASH("transaction 2 type = %s",transaction_type2);
//   }
//
//   //---------------------------------------------------------------
//	 //capture the token   ---next ten digits...
//	 indx++; // :
//	 indx++; //  next field
//	 int i;
//	 for( i = 0 ;i<10;i++)
//	 {
//        token_[i] = mainMessage.msg[indx];
//        indx++;
//	 }
//	 token_[i] = 0; // terminate string.
//
//	// DEBUG_FLASH("token === %s",token_);
////---------------------------------------------------------------
//	 // capture the attendants tag.
//	 indx++; //  next field
//	 i = 0;
//	 while ( mainMessage.msg[indx] != ':' )
//	 	{
//			  if(pump_indx == 0)
//			  {
//				tag[i] = mainMessage.msg[indx];
//			  }
//			  else
//			  {
//				tag2[i] = mainMessage.msg[indx];
//			  }
//	 		indx++;
//	 		i++;
//	 	}
//	 	tag[i] = 0;   //terminate
//	 	tag2[i] = 0;   //terminate
//
//	 //	 DEBUG_FLASH("TAG 1 = %s",tag);
//	 //  DEBUG_FLASH("TAG 2 = %s",tag2);
////---------------------------------------------------------------
//	 	 indx++;   // next field
//
//	 //capture the amount...
//	 uint8_t indx_ = 0;
//	  while (mainMessage.msg[indx] != 0)
//	  {
//		amount[indx_] =  mainMessage.msg[indx];
//		indx++;
//		indx_++;
//	  }
//	  amount[indx_] = 0;     //terminate the string.
//
//	  DEBUG_FLASH(" Amount === %s",amount);
////=======================================================
//	  //go to authorise the pump with price
//	  //pump[]
//	  float amt = 0.0;
//	  amt =  atof((char*)amount);
//	  //---------------------------------------------
//		if (amt < 0)
//			{
//				// no amount was sent.....
//				DEBUG_FLASH(" No Amount was sent...");
//				amt = 0.0;     //set amt to zero.
//
//				// wait for reply from server...
//               memcpy(cb[pump_indx].pump_name,pump_name,3);
//               memcpy(cb[pump_indx].token_,token_,12);
//               memcpy(cb[pump_indx].transaction_type,transaction_type,2);
//               cb[pump_indx].auth_type = auth_price;
//               cb[pump_indx].wait_for_server = 1;
//
//               server_stat = server_busy;
//
//               //   go ask the server for the voucher amount...
//            	verify('v',pump_name,token_);
//
//               return;
//				//-----------------------------------------------
//			}
//		else
//			{
//				//amt =  atof((char*)amount);
//			    DEBUG_FLASH("auth of %2.f ",amt);
//			}
//
//	//              Send authorisation to pump.
//	//                 Go to access the pump
////=======================================================
//
//  ack_stat = send_command(_auth_amt, pump_name, amt);
//
////========================================================
//exit_auth_amount:
//
//                    msg_str.head = '[';
//		     	 	msg_str.tail = ']';
//		     	 	msg_str.cmd[0] = '1'; //== stop command is 04.
//		     	 	msg_str.cmd[1] = '2'; //
//		     	 	msg_str.cmd[2] = 0;   //   terminate string.
//		     	  memcpy(msg_str.id, mainMessage.id,strlen(mainMessage.id));
//		     	//   msg_str.msg = msg;
//
//     sprintf(msg_str.msg,"%s:%s,%d",pump_name,token_,ack_stat);
//
//			   msg_str.ln_ = strlen(msg_str.msg);
//
//		 	  msg_str.msg[ msg_str.ln_] = 0;
//		 	  msg_str.msg[ msg_str.ln_++] = 0;  //null termination.
//
//
//
////	 strcpy(msg_str.msg , pump_name);
////	 msg+=':';     //end of pump_name;
////	 msg+=token_;
//
////	 uint8_t ln_ = strlen(mainMessage.id);
////
////	 msg[ln_] = 48 + ack_stat;
////     msg[ln_+ 1] = 0;
//
//
//	//  DEBUG_FLASH("msg b4 build -- %s",msg);
//
////	  string output = "";
////
////	  msg_str.ln_ = strlen((char*)msg.c_str());
////	  int indxx;
////	  for(indxx = 0;indxx<msg_str.ln_;indxx++)
////		  {
////			msg_str.msg_[indxx] = msg[indxx];
////		  }
////
////	  msg_str.msg_[indxx] = 0;
////	  msg_str.msg_[indxx++] = 0; //null termination.
////
////	 // output = "";
////	 // output =   mainMessage.BuildString(mainMessage);
////	  output =   mainMessage.BuildString();
////
////	  DEBUG_FLASH("lenght %s",output);
////
////	   uint8_t ln = strlen((char*)output.c_str());
////     //  output[ln] = 0;
////	 //send
////	  DEBUG_FLASH("lenght %d",ln);
////	  uartSendData( (char*)output.c_str(), ln );
//}
//
//
//
//
//
//
//
