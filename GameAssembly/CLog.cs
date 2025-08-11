using System;
using System.Runtime.CompilerServices;

namespace GameAssembly
{
    public class CLog : IDisposable
    {
        private IntPtr nativePtr;

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr Create(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void NLog(IntPtr ptr, string message);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Destroy(IntPtr ptr);

        public CLog(string name)
        {
            nativePtr = Create(name);
        }

        public void Log(string message)
        {
            NLog(nativePtr, message);
        }

        public void Dispose()
        {
            if (nativePtr != IntPtr.Zero)
            {
                Destroy(nativePtr);
                nativePtr = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }

        ~CLog()
        {
            Dispose(); // 안전하게 가비지 컬렉션 시 네이티브 메모리도 해제
        }
    }
}
