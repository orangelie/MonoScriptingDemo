using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameAssembly
{
    public class MonoBehaviour
    {
        public virtual void Start() { }
        public virtual void Update() { }
    }

    public class Player : MonoBehaviour
    {
        public override void Start()
        {
            System.Console.WriteLine("Player Start!");
        }

        public override void Update()
        {
            System.Console.WriteLine("Player Update!");
        }
    }
}
