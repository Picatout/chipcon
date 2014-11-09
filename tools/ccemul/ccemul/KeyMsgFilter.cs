/*
 * Created by SharpDevelop.
 * User: Jacques
 * Date: 2014-11-08
 * Time: 22:02
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 * 
 * REF: http://stackoverflow.com/questions/10267034/keydown-event-is-not-firing-keypreview-set-to-true
 */
using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace ccemul
{
	/// <summary>
	/// ajout d'un filtre pour capture un message windows
	/// </summary>
	public class KeyMessageFilter : IMessageFilter
	{
	    private enum KeyMessages
	    {
	        WM_KEYFIRST = 0x100,
	        WM_KEYDOWN = 0x100,
	        WM_KEYUP = 0x101,
	        WM_CHAR = 0x102,
	        WM_SYSKEYDOWN = 0x0104,
	        WM_SYSKEYUP = 0x0105,
	        WM_SYSCHAR = 0x0106,
	    }
	
	    [DllImport("user32.dll")]
	    private static extern IntPtr GetParent(IntPtr hwnd);
	
	    // We check the events agains this control to only handle
	    // key event that happend inside this control.
	    Control _control;
	
	    public KeyMessageFilter()
	    { }
	
	    public KeyMessageFilter(Control c)
	    {
	        _control = c;
	    }
	
	    public bool PreFilterMessage(ref Message m)
	    {
	        if (m.Msg == (int)KeyMessages.WM_KEYDOWN)
	        {
	            if (_control != null)
	            {
	                IntPtr hwnd = m.HWnd;
	                IntPtr handle = _control.Handle;
	                while (hwnd != IntPtr.Zero && handle != hwnd)
	                {
	                    hwnd = GetParent(hwnd);
	                }
	                if (hwnd == IntPtr.Zero) // Didn't found the window. We are not interested in the event.
	                    return false;
	            }
	            Keys key = (Keys)m.WParam;
	            switch (key)
	            {
	                case Keys.Left:
	                    MessageBox.Show("Left");
	                    return true;
	                case Keys.Right:
	                    MessageBox.Show("Right");
	                    return true;
	            }
	        }
	        return false;
	    }
	}
}
