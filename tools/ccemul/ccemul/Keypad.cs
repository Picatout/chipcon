﻿/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-07
 * Time: 14:54
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
using System;
using System.Threading;
	
namespace ccemul
{
	/// <summary>
	/// Description of Keypad.
	/// </summary>
	public class Keypad
	{
		
		byte[] hkeys=new byte[16]
		{
		88, // X
		49, // 1
		50, // 2
		51, // 3
		81, // Q
		87, // W
		69, // E
		65, // A
		83, // S
		68, // D
		52, // 4
		82, // R
		70, // F
		86, // V
		90, // Z
		67, // C
		};
		
		internal ushort keys_state=0; // état des touches
		internal bool fWaitKey=false;
		
		public Keypad()
		{
		}

		
		internal byte keypadRead()
		{
			for (byte i=0;i<16;i++){
				if ((keys_state & (1<<i))==(1<<i))
				{
					//keys_state &=(ushort)(~(1<<i)&0xffff);
					return i;
				}
			}
			return 255;
		}
		
		internal byte waitKey()
		{
			if (keys_state==0)
			{
				fWaitKey=true;
				return 255;
			}else
				return keypadRead();
		}

		internal bool hexKeyDown(byte k)
		{
			byte i;
			
			for (i=0;i<16;i++) if (hkeys[i]==k)
			{				
				keys_state|=(ushort)(1<<i);
				return true;
			}
			return false;
		}
		
		internal bool hexKeyUp(byte k)
		{
			byte i=0;
			
			for (i=0;i<16;i++) if (hkeys[i]==k)
			{				
				keys_state &=(ushort)(~(1<<i)&0xffff);
				return true;
			}
			return false;
		}
		
		/*
 
		void prompt_key()
		{
			
		}
		
		byte keypad_break(){
			return 0;
		}
*/		
		internal bool keyDown(byte k)
		{
			return (keys_state & (1<<k))==(1<<k);
		}
	}
}
