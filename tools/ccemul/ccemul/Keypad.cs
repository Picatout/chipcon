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
using System.Windows.Input;
	
namespace ccemul
{
	/// <summary>
	/// Description of Keypad.
	/// </summary>
	public class Keypad
	{
		Key[] kpad=new Key[16]
		{
		Key.X,
		Key.D1,
		Key.D2,
		Key.D3,
		Key.Q,
		Key.W,
		Key.E,
		Key.A,
		Key.S,
		Key.D,
		Key.D4,
		Key.R,
		Key.F,
		Key.V,
		Key.Z,
		Key.C
		};
		
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
		
		public Keypad()
		{
		}
		
		byte keypad_read()
		{
			return 0;
		}
		
		byte wait_key()
		{
			return 0;
		}

		internal byte hexKey(byte k)
		{
			byte i=0;
			
			for (i=0;i<16;i++) if (hkeys[i]==k) break;
			return i;
		}
		
		/*
 
		void prompt_key()
		{
			
		}
		
		byte keypad_break(){
			return 0;
		}
*/		
		bool key_down(byte k)
		{
			return Keyboard.IsKeyDown(kpad[k]);
		}
		
	}
}
