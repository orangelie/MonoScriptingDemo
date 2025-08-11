using System.IO;

namespace GameAssembly
{
    public class MonoBehaviour
    {
        public virtual void Start() { }
        public virtual void Update() { }
    }

    public class Player : MonoBehaviour
    {
        private CLog log = new CLog("test1");
        private int hitCount = 0;

        public override void Start()
        {
            System.Console.WriteLine("[C#] Player Start!");
            File.WriteAllText("test.txt", "Hello World !");
        }

        public override void Update()
        {
            System.Console.WriteLine("[C#] Player Update!");
            log.Log($"Hello World ! => {hitCount++}");
        }
    }
}
