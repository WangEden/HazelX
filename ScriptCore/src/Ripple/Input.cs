using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Ripple
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return IsKeyPressed_Native(keyCode);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool IsKeyPressed_Native(KeyCode keyCode);
    }
}