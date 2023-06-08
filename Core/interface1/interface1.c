///*
// * nextion2.c
// *
// *  Created on: Oct 14, 2022
// *      Author: tunjo
// */
//
//#include "main.h"
//#include "stdbool.h"
//#include "interface1.h"
//#include "stdint.h"
//
//#include "config.h"
//#include "checksum.h"
//#include "pump_comm.h"
//#include "math.h"
//
//#include "interface_fxn1.h"
//
//
////#define uint8_t uint8_t
//
//extern UART_HandleTypeDef huart3;
//
//char nextion1_rx_buf[nextion_rx_bufsize] = {0};
//char interf_buf1[nextion_rx_bufsize] = {0};
//
//bool message_found = 0;
//bool parse_ready1 = 0;
//
//bool message_found2 = 0;
//bool parse_ready2 = 0;
//
//bool isDirty = 0;
//
//msgg  retMsg[2] , mainMessage;     //], retMsg2;
//extern config_struct config_data;
//extern rcv_struct  msg_str;
//
//char my_message[400] = {0};
////char msg_[400] = {0};
//
//
////=========================
//void nextion1_ini()
//{
//	 HAL_UART_Receive_IT(&huart3, nextion1_rx_buf, nextion_rx_bufsize );
//}
////==========================
//void nextion1_rx()
//{
//	static int header_found = 0;
//
//     int rxcnt = nextion_rx_bufsize - huart3.RxXferCount;
//     int tx = nextion1_rx_buf[rxcnt-1];
//
//
//     if(tx == '[') header_found = 1;
//
//     if ( (header_found != 1) )
//     {
////    	  huart3.RxXferCount = nextion_rx_bufsize;
////    	  huart3.pRxBuffPtr = &nextion1_rx_buf[0]; //reset
////    	  rxcnt = 0;
//     }
//
//     if ( (header_found == 1) )
//        {
//    	   if(rxcnt >= nextion_rx_bufsize - 2) //if over receive
//    	   {
//    	    	 huart3.RxXferCount = nextion_rx_bufsize;
//    	    	 huart3.pRxBuffPtr = &nextion1_rx_buf[0]; //reset
//    	    	 rxcnt = 0;
//    	    	 header_found = 0;
//    	   }
//    	 	 //header found lookmfor the footer...
//    	   if(tx == ']')
//			{
//			   header_found = 0;
//			   memcpy( interf_buf1 , nextion1_rx_buf ,rxcnt ); //transfer to the definitive buffer.
//			   huart3.RxXferCount = nextion_rx_bufsize;
//				 huart3.pRxBuffPtr = &nextion1_rx_buf[0]; //reset
//				 rxcnt = 0;
//				 header_found = 0;
//				 message_found = 1;
//			}
//        }
//     //--------------------------------------------------------
//          if ( (rxcnt >= 30)&&(header_found != 1) )
//          {
//         	 huart3.RxXferCount = nextion_rx_bufsize;
//         	 huart3.pRxBuffPtr = &nextion1_rx_buf[0]; //reset
//         	 rxcnt = 0;
//          }
//     //--------------------------------------------------------
//}
//
//
//bool msg_parse(char* str,int n_)
// {
//	uint8_t   head_pos = 0;
//	char di[17] = {0};
//	char cmd[3] = {0};
//	char msg[200] = {0};       //string msg = "";
//	char tail ;
//	char headr;
//	char crc[3] = {0};
//
//	uint8_t  crc_rX_8 = 0;
//
//	uint8_t indx_tail = 0;
////------------------------------------------
//	char buf_crc[150] = {0};
//
//	isDirty = false ;                //  reset the dirty flag
//	int str_len = strlen(str);
//
//	if(str_len <19)
//	{
//		isDirty = true;
//		return fail;
//	}
//
//	int indx = 0;
//	while(indx < str_len)
//	{
//		if(str[indx] != '[')
//		{
//			indx++;
//		}
//		else
//		{
//			//eat_trace("got header @ %d ",indx);
//			head_pos = indx;
//			headr = '[';
//			goto got_header;
//		}
//	}
//	indx++;   //advance index
//	//eat_trace("Header not found! ");
//	isDirty = true;
//	return fail;
//got_header:
//
//     //check tail.
//      indx_tail = 0;
//while(indx_tail < str_len)
//	{
//		if(str[indx_tail] != ']')
//		{
//			indx_tail++;
//		}
//		else
//		{
//			//eat_trace("got tail @ %d ",indx_tail);
//			tail = ']';
//			goto got_tail;
//		}
//	}
//	//eat_trace("tail not found! ");
//	isDirty = true;
//	return fail;
//got_tail:
//   //uint8_t j = 0;
//   // imei
//   // for(int indx_ = 0;indx_<15;indx_++)
//indx++;
//int indx_ = 0;
//  while(indx_ < 15)
//	{
//		di[indx_] = str[indx];  //extract 15 characters for imei
//		indx_++;
//		indx++;
//	}
//	di[indx_] = 0;  //null terminate.
////	eat_trace(" di  %s", di);
//	//============================================================
//	//   find out if the right registered device is communicating
//	//int gid_cfg_len =
//
//	int ret = strcmp(config_data.others.gid, di);
//
//	 if (ret != 0 )
//		 {
//			 //==== Device not found ====
//			// eat_trace("Device NOT found! ");
//			 isDirty = true;
//			 return fail;
//		 }
//	// eat_trace("Device found ");
//	//============================================================
//
////command.
//	indx_ = 0;
//	  while(indx_ < 2)
//		{
//			cmd[indx_] = str[indx];  //extract 2 characters for cmd
//			indx_++;
//			indx++;
//		}
//		cmd[indx_] = 0;  //null terminate.
//		//eat_trace(" cmd  %s", cmd);
//
////message.
//		indx_ = 0;
//			  while(indx < indx_tail-2)  //to cater for the 2xters of crc.
//				{
//					msg[indx_] = str[indx];  //extract 2 characters for cmd
//					indx_++;
//					indx++;
//				}
//		msg[indx_] = 0;  //null terminate.
//		//eat_trace(" msg  %s", msg);
////-----------------------------------------------------------------------
////Receive CRC.
//	indx_ = 0;
//	  while(indx < indx_tail)
//		{
//			crc[indx_] = str[indx];  //extract 2 characters for crc
//			indx_++;
//			indx++;
//		}
//	crc[indx_] = 0;      //NULL terminate.
//	//eat_trace("Received crc(hex) = %s", crc);
//
//	    char *end;
//	    crc_rX_8 = strtoll(crc, &end, 16);
//
//       //  crc_rX_8 = atoi((char*)crc);
//
//      //  eat_trace("decimal crc = %d", crc_rX_8);
////-----------------------------------------------------------------------
////  get all the received elements so far into a buffer.
////  copy from index of  head_pos to current indx.
//
//  uint8_t temp = 0;
//	for ( indx_ = head_pos  ; indx_ < indx_tail-2 ; indx_++) //start to before crc.
//	{
//	  buf_crc[temp] = str[indx_];
//	  temp++;
//	}
//	  buf_crc[temp] = 0;            //NULL terminate
//
//	//	eat_trace(" packet so far: %s", buf_crc);
//	//	eat_trace(" lenght = %d, strlen = %d", indx - 1-2, strlen(buf_crc));
//
////calculate	the CRC...
//		int lenn = strlen(buf_crc);
//      uint8_t  crc_calc = crc_8((uint8_t*) buf_crc, lenn );
//
//     // eat_trace("calculated CRC = %0x  , decimal --> %d",crc_calc,crc_calc);
//
//     if(crc_calc != crc_rX_8)
//     {
//    	// eat_trace(" crc error !");
//    	 isDirty = true;
//    	 return fail;
//     }
//   //  eat_trace(" crc OK !");
//
//
//// copy to the message structure...
//if (n_ == 1)   //store into the nextion1 structure...
//	{
//		 strncpy(retMsg[n_].id  , di,strlen(di) );	   //eat_trace("Ret di %s", retMsg.id);
//		 strncpy(retMsg[n_].cmd , cmd,strlen(cmd) );   //eat_trace("Ret cmd %s", retMsg.cmd);
//		 strncpy(retMsg[n_].msg , msg,strlen(msg) );
//		 if (n_ == 1)   //store into the nextion1 structure...
//		 	{
//			    parse_ready1 == 1;
//		 	} else
//		 		 parse_ready2 == 1;
//	}
//
// return ok;
//}
//
////-------------------------------------------------------------
//
//
//void BuildString() ///message msg)
//{
//	char crc[4] = {0};
//
//  //----------------------------------------------------------
//  // add header
//   uint16_t index_  = 1;
//   my_message[0] = '[';
//
//   uint16_t ui = 0;
//  //-----------------------------------------------------------
//  //add id.
//  //-----------------------------------------------------------
//   index_ = strlen(my_message);
//
//   for(ui = 0;ui<15;ui++)
//   {
//	   my_message[ui+1] = msg_str.id[ui];
//	   index_++;
//   }
//// memcpy(my_message+index_,(char*)msg_str.id.c_str(),index_);
////--------------------------------------------------------------
//    //add cmd.
//   uint16_t temp = index_;
//   for(ui = 0;ui<2;ui++)
//      {
//   	    my_message[ui+temp] = msg_str.cmd[ui];
//   	    index_++;
//      }
//  //------------------------------------------------------------
//  //add msg.
// temp = index_;
//
// uint8_t lenn_ = strlen(my_message);
//  for(ui = 0;ui<lenn_;ui++)
//     {
//  	    my_message[ui+temp] = msg_str.msg[ui];
//  	    index_++;
//     }
// // eat_trace("index is ==> %d ",  index_ );
//
////--------------------------------------------------------------
// //calculate crc
//
//int lenn =  strlen(my_message);
//
// uint8_t calc_crc = crc_8((uint8_t*)my_message, lenn);
//
////  eat_trace("calc. CRC is ==> %d == > %0x", calc_crc,calc_crc);
//
//  sprintf(crc,"%0x",calc_crc );   //eat_trace("CRC str -%s ", crc);
//
//  memcpy(my_message+index_,crc,2);
//
////  eat_trace("packt is ==> %s ", my_message);
//
//index_ = strlen(my_message);
//my_message[index_] = ']';
//my_message[index_+1] = 0;
//
////eat_trace("packt is ==> %s ", my_message);
////return my_message;
//
//}
//
////=============================================================
//
//void uartSendData(char* output)
//{
//	HAL_UART_Transmit_it(huart3,output,strlen(output));
//}
//
//
////=============================================================
//
//int get_pump_index_from_name_string(char* pn)
//{
//	for (int i=0; i<TOTAL_NO_OF_PUMPS; i++)
//	{
//			if (strncmp(pn, config_data.pump[i].name, strlen(pn)) )
//			{
//				return i;
//			}
//	}
//		return -1;
//}
//
////============================================================
////=========================================================================
//void write_att_tag(int fxn)
//{
////	//unsigned short att_file_name[20];
////	unsigned int writn = 0;
////	int ret;
////
////	//memcpy(att_file_name, AT_TAG_FILE_NAME,sizeof(att_file_name)/sizeof(att_file_name[0]));
////
////	TRACE_FS("<%s>", __FUNCTION__);
////	int File_handle;
////	if (fxn != clear)
////	{
////		File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME,FS_CREATE);
////
////		if (File_handle < EAT_FS_NO_ERROR)
////			{
////				DEBUG_MAIN("<%s> error opening att_tag file", __FUNCTION__);
////				return ;     //File_handle;
////			}
////	}
////	else
////	{
////		int File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME,FS_CREATE); //clear file.
////		 ret = eat_fs_Write(File_handle, (char*)config_data.others.atag.c_str(),1,&writn);
////		//ret = eat_fs_Truncate(File_handle);
////		ret = eat_fs_Commit(File_handle);
////			if (ret != EAT_FS_NO_ERROR)	TRACE_FS("!!!! --- ERROR! in COMMITTING to file");
////				eat_fs_Close(File_handle);
////				return;
////	}
////
////
////
////	//int ret = eat_fs_Seek(File_handle, 0, EAT_FS_FILE_BEGIN);
////	//(FS_HANDLE FileHandle, void * DataPtr, UINT Length, UINT * Written);
////
////	 ret = eat_fs_Write(File_handle, (char*)config_data.others.atag.c_str(),config_data.others.atag.length(),&writn);
////
////	if (ret != EAT_FS_NO_ERROR)
////	{
////		DEBUG_MAIN("<%s> Error writing tag into att_tag file", __FUNCTION__);
////		return ;
////	}
////	else
////	{
////		//DEBUG_MAIN("<%s> writing into att_tag file", __FUNCTION__);
////
////		ret = eat_fs_Commit(File_handle);
////		if (ret != EAT_FS_NO_ERROR)
////		{
////			TRACE_FS("!!!! --- ERROR! in COMMITTING to file");
////			eat_fs_Close(File_handle);
////			return;
////		}
////			 TRACE_FS("att. file written and saved");
////			return;
////	}
////
////	return ;
//
//}
////=========================================================================
//void write_att_tag2(int fxn)
//{
////	//unsigned short att_file_name[20];
////	unsigned int writn = 0;
////	int ret;
////
////	//memcpy(att_file_name, AT_TAG_FILE_NAME,sizeof(att_file_name)/sizeof(att_file_name[0]));
////
////	TRACE_FS("<%s>", __FUNCTION__);
////	int File_handle;
////	if (fxn != clear)
////	{
////		File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME2,FS_CREATE);
////
////		if (File_handle < EAT_FS_NO_ERROR)
////			{
////				DEBUG_MAIN("<%s> error opening att_tag 2 file", __FUNCTION__);
////				return ;     //File_handle;
////			}
////	}
////	else
////	{
////		int File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME2,FS_CREATE); //clear file.
////		 ret = eat_fs_Write(File_handle, (char*)config_data.others.atag2.c_str(),1,&writn);
////		//ret = eat_fs_Truncate(File_handle);
////		ret = eat_fs_Commit(File_handle);
////			if (ret != EAT_FS_NO_ERROR)	TRACE_FS("!!!! --- ERROR! in COMMITTING to file 2");
////				eat_fs_Close(File_handle);
////				return;
////	}
////
////
////
////	//int ret = eat_fs_Seek(File_handle, 0, EAT_FS_FILE_BEGIN);
////	//(FS_HANDLE FileHandle, void * DataPtr, UINT Length, UINT * Written);
////
////	 ret = eat_fs_Write(File_handle, (char*)config_data.others.atag2.c_str(),config_data.others.atag2.length(),&writn);
////
////	if (ret != EAT_FS_NO_ERROR)
////	{
////		DEBUG_MAIN("<%s> Error writing tag into att_tag 2 file", __FUNCTION__);
////		return ;
////	}
////	else
////	{
////		//DEBUG_MAIN("<%s> writing into att_tag file", __FUNCTION__);
////
////		ret = eat_fs_Commit(File_handle);
////		if (ret != EAT_FS_NO_ERROR)
////		{
////			TRACE_FS("!!!! --- ERROR! in COMMITTING to file 2");
////			eat_fs_Close(File_handle);
////			return;
////		}
////			 TRACE_FS("att. file 2 written and saved");
////			return;
////	}
////
////	return ;
//
//}
////======================================================================================
//void read_att_tag2()
//{
////	//unsigned short att_file_name[20];
////
////	//memcpy(att_file_name, AT_TAG_FILE_NAME,sizeof(att_file_name)/sizeof(att_file_name[0]));
////
////	TRACE_FS("<%s>", __FUNCTION__);
////	int File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME2,FS_READ_ONLY);
////
////	if (File_handle < EAT_FS_NO_ERROR)
////	{
////		DEBUG_MAIN("<%s> error opening att_tag file2", __FUNCTION__);
////		config_data.others.atag2 = "";
////		return ;     //File_handle;
////	}
////
////	//int ret = eat_fs_Seek(File_handle, 0, EAT_FS_FILE_BEGIN);
////	//(FS_HANDLE FileHandle, void * DataPtr, UINT Length, UINT * Written);
////    unsigned int writn = 0;
////    string fstr = "";
////    char c;
////    uint8_t n = 0;
////    int ret ;
////
////		while ( (c != 0) && (n < 15) )
////		{
////		   ret = eat_fs_Read(File_handle, &c,1,&writn);
////		   // DEBUG_MAIN("<%s> c== : %c", __FUNCTION__,c);
////
////		   if (ret != EAT_FS_NO_ERROR)
////			   {
////			   	   eat_fs_Close(File_handle);
////			   	   break;
////			   }
////		   fstr+=c;
////		   n++;
////		}
////		eat_fs_Close(File_handle);
////		int ln = fstr.length();
////   if(fstr[0] != 0)
////   {
////	   	DEBUG_MAIN("     ");
////		DEBUG_MAIN("<%s>ATTENDANT 2 TAG ===========>: %s   <still logged in! lenght %d>", __FUNCTION__,fstr,ln);
////		DEBUG_MAIN("     ");
////		//config_data.others.atag = fstr;
////		int i;
////       for( i = 0;i<ln;i++)
////       {
////    	   config_data.others.atag2+= fstr[i];
////    	   attendant_2[i] = fstr[i];
////    	   //config_data.others.atag[i+1] = 0;
////       }
////       config_data.others.atag2[i] = 0;
////       attendant_2[i] = 0;    //NULL terminator.
////
////     DEBUG_MAIN("<%s>ATTENDANT 2 TAG IN CONFIG====>: %s   lenght %d>", __FUNCTION__,config_data.others.atag2,ln);
////   }
////   else
////   {
////	    DEBUG_MAIN("     ");
////		DEBUG_MAIN("<%s> ATTENDANT2 NOT LOGGED IN !>", __FUNCTION__);
////		DEBUG_MAIN("     ");
////		config_data.others.atag2 = "";
////   }
////
////	return ;
//}
////======================================================================================
//void read_att_tag()
//{
////	//unsigned short att_file_name[20];
////
////	//memcpy(att_file_name, AT_TAG_FILE_NAME,sizeof(att_file_name)/sizeof(att_file_name[0]));
////
////	TRACE_FS("<%s>", __FUNCTION__);
////	int File_handle = eat_fs_Open((unsigned short*)AT_TAG_FILE_NAME,FS_READ_ONLY);
////
////	if (File_handle < EAT_FS_NO_ERROR)
////	{
////		DEBUG_MAIN("<%s> error opening att_tag1 file", __FUNCTION__);
////		config_data.others.atag = "";
////		return ;     //File_handle;
////	}
////
////	//int ret = eat_fs_Seek(File_handle, 0, EAT_FS_FILE_BEGIN);
////	//(FS_HANDLE FileHandle, void * DataPtr, UINT Length, UINT * Written);
////    unsigned int writn = 0;
////    string fstr = "";
////    char c;
////    uint8_t n = 0;
////    int ret ;
////
////		while ( (c != 0) && (n < 15) )
////		{
////		   ret = eat_fs_Read(File_handle, &c,1,&writn);
////		   // DEBUG_MAIN("<%s> c== : %c", __FUNCTION__,c);
////
////		   if (ret != EAT_FS_NO_ERROR)
////			   {
////			   	   eat_fs_Close(File_handle);
////			   	   break;
////			   }
////		   fstr+=c;
////		   n++;
////		}
////		eat_fs_Close(File_handle);
////		int ln = fstr.length();
////   if(fstr[0] != 0)
////   {
////	   	DEBUG_MAIN("     ");
////		DEBUG_MAIN("<%s>ATTENDANT 1 TAG ===========>: %s   <still logged in! lenght %d>", __FUNCTION__,fstr,ln);
////		DEBUG_MAIN("     ");
////		//config_data.others.atag = fstr;
////		int i;
////       for( i = 0;i<ln;i++)
////       {
////    	   config_data.others.atag+= fstr[i];
////    	   attendant_[i] = fstr[i];
////    	   //config_data.others.atag[i+1] = 0;
////       }
////       config_data.others.atag[i] = 0;
////       attendant_[i] = 0;    //NULL terminator.
////
////     DEBUG_MAIN("<%s>ATTENDANT 1 TAG IN CONFIG====>: %s   lenght %d>", __FUNCTION__,config_data.others.atag,ln);
////   }
////   else
////   {
////	    DEBUG_MAIN("     ");
////		DEBUG_MAIN("<%s>ATTENDANT 1 NOT LOGGED IN !>", __FUNCTION__);
////		DEBUG_MAIN("     ");
////		config_data.others.atag = "";
////   }
////
////	return ;
//}
//
//
//void GO_fsm1()
//{
////	not_defined,
////		get_status = 1,
////		auth_amt,
////		auth_vol,
////		stop_cmd,
////		pause_cmd,
////		resume_cmd,
////		switch_mode_cmd,
////		login_cmd,
////		logout_cmd,
//
//  if (parse_ready1 == 0)  return;
//
//     int cmmd =  atoi((char*)retMsg[0].cmd);
//
//
//
//  switch(cmmd)
//	{
//
//	case _get_status:
//		status_();
//		//pump[0]->my_log("0011", 100, 0, 40.5, 0,180.00, 34002, 'a', "01234567890");
////				(tid_temp, amo_temp, tz_difference, current_tz, 0,
////					unp_temp, time_temp, tty_temp, tag_temp);
//		break;
//
//	case _auth_amt:
//		auth_amount();
//		break;
//
//	case _auth_vol:
//		auth_volume();
//			break;
//
//	case _stop_cmd:
//		stop_cmd();
//			break;
//
//	case _pause_cmd:
//		pause_cmd();
//			break;
//
//	case _resume_cmd:
//		resume_cmd();
//			break;
//
//	case _switch_mode_cmd:
//		switch_mode_cmd();
//			break;
//
//	case _login_cmd:
//		login_cmd();
//			break;
//
//	case _logout_cmd:
//		logout_cmd();
//			break;
//	case _server_resp:
//		send_server();
//		break;
//	case _who:
//		send_attendants();
//		break;
//	}
//
//}
//
