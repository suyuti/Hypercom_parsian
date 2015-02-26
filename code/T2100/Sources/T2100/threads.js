function getState(job)
{
  if (job.bAlloc==0)
    return "free";
  else if (job.bWaitRes!=0)
    return "resource wait"
  else if (job.bWait)
    return "event wait";
  else if (job.bSignal)      
    return "signalled";
  else
    return "signal wait";
}

function getregs(sp)
{
  var a = new Array();

  a[0] = Debug.evaluate("*(unsigned*)0x"+(sp+0x10).toString(16));

  a[1] = 0;
  a[2] = 0;
  a[3] = 0;

  a[4] = Debug.evaluate("*(unsigned*)0x"+sp.toString(16));
  a[5] = Debug.evaluate("*(unsigned*)0x"+(sp+4).toString(16));
  a[6] = Debug.evaluate("*(unsigned*)0x"+(sp+8).toString(16));
  a[7] = Debug.evaluate("*(unsigned*)0x"+(sp+0xc).toString(16));
  a[8] = Debug.evaluate("*(unsigned*)0x"+(sp+0x30).toString(16));
  a[9] = Debug.evaluate("*(unsigned*)0x"+(sp+0x14).toString(16));
  a[10] = Debug.evaluate("*(unsigned*)0x"+(sp+0x18).toString(16));
  a[11] = Debug.evaluate("*(unsigned*)0x"+(sp+0x1c).toString(16));

  a[12] = Debug.evaluate("*(unsigned*)0x"+(sp+0x38).toString(16));

  a[13] = a[12];
  a[14] = Debug.evaluate("*(unsigned*)0x"+(sp+0x3c).toString(16));
  a[15] = Debug.evaluate("*(unsigned*)0x"+(sp+0x40).toString(16));

  a[16] = 0;

  return a;
}

function update() 
{
  Threads.clear();
  var current=Debug.evaluate("nJobNrCurr");
  var jobs=Debug.evaluate("O_JobData");
  Threads.newqueue("Executing");
  Threads.add(current.toString(10), 0, "running", []);
  Threads.newqueue("Waiting");
  var priority = 1;
  for (i=current+1;i<64;i++)
    {
      if (getState(jobs[i])!="free")
        {
          priority++;
        }
    }
  for (i=0;i<current;i++)
    {
      if (getState(jobs[i])!="free")
        {
          Threads.add(i.toString(10), priority, getState(jobs[i]), getregs(jobs[i].SP));
          priority++;
        }
    }
  priority = 1;
  for (i=current+1;i<64;i++)
    {
      if (getState(jobs[i])!="free")
        {
          Threads.add(i.toString(10), priority, getState(jobs[i]), getregs(jobs[i].SP));
          priority++;
        }
    }
}
