/*
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
		private KeyMessageFilter m_filter = new KeyMessageFilter();
		
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
		
		byte[] key_queue=new byte[256];
		byte queueHead=0, queueTail=0;
		
		public Keypad()
		{
		}

/*		
		byte keypad_read()
		{
			return 0;
		}
*/		
		internal byte waitKey()
		{
			while (queueHead==queueTail);
			return key_queue[queueTail++];
		}

		internal bool hexKey(byte k)
		{
			byte i=0;
			
			for (i=0;i<16;i++) if (hkeys[i]==k)
			{				
				key_queue[queueHead++]=i;
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
			while (queueHead!=queueTail)
			{
				if (key_queue[queueTail]==k)
				{
					queueTail++;
					return true;
				}
				queueTail++;
			}
			return false;
		}
	}
}
