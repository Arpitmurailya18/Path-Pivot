const canvas=document.getElementById('canvas'),ctx=canvas.getContext('2d');
const $=id=>document.getElementById(id);
const sortNames=['Bubble Sort','Selection Sort','Insertion Sort','Merge Sort','Quick Sort'];
const pathNames=['BFS','DFS','Dijkstra','A* Search'];
const pseudo={
'Bubble Sort':['procedure bubbleSort(A)',' n = length(A)',' repeat','  swapped = false','  for i = 1 to n-1 do','   if A[i-1] > A[i] then','    swap(A[i-1], A[i])','    swapped = true','   end if','  end for','  n = n - 1',' until not swapped','end procedure'],
'Selection Sort':['procedure selectionSort(A)',' n = length(A)',' for i = 0 to n - 1 do','  minIndex = i','  for j = i + 1 to n do','   if A[j] < A[minIndex] then','    minIndex = j','   end if','  end for','  swap(A[i], A[minIndex])',' end for','end procedure'],
'Insertion Sort':['procedure insertionSort(A)',' for i = 1 to length(A) - 1 do','  key = A[i]','  j = i - 1','  while j >= 0 and A[j] > key do','   A[j+1] = A[j]','   j = j - 1','  end while','  A[j+1] = key',' end for','end procedure'],
'Merge Sort':['procedure mergeSort(A)',' for curr_size = 1 to n-1 by 2*curr_size','  for left_start = 0 to n-1 by 2*curr_size','   mid = ...','   right_end = ...','   merge(A,left_start,mid,right_end)','  end for',' end for','end procedure'],
'Quick Sort':['procedure quickSort(A,low,high)',' if low < high','  p = partition(A, low, high)','  quickSort(A, low, p - 1)','  quickSort(A, p + 1, high)',' end if','end procedure','procedure partition(A,low,high)',' pivot = A[high]',' i = low - 1',' for j = low to high - 1','  if A[j] < pivot','   i++','   swap(A[i], A[j])','  end if',' end for',' swap(A[i+1], A[high])',' return i + 1','end procedure'],
'BFS':['procedure BFS(graph,start,end)',' let Q be a queue',' Q.enqueue(start)',' mark start as visited',' while Q is not empty do','  current = Q.dequeue()','  if current is end then','   return PathFound','  end if','  for each neighbor of current do','   if neighbor is not visited then','     mark neighbor as visited','     Q.enqueue(neighbor)','   end if','  end for',' end while',' return PathNotFound','end procedure'],
'DFS':['procedure DFS(graph,start,end)',' let S be a stack',' S.push(start)',' while S is not empty do','  current = S.pop()','  if current is not visited then','    mark current as visited','    if current is end then','      return PathFound','    end if','    for each neighbor of current do','      S.push(neighbor)','    end for','  end if',' end while',' return PathNotFound','end procedure'],
'Dijkstra':['procedure Dijkstra(graph, start, end)',' dist[source] = 0',' create vertex priority queue Q',' while Q is not empty','  u = vertex in Q with min distance','  remove u from Q','  for each neighbor v of u','   alt = dist[u] + length(u, v)','   if alt < dist[v]','    dist[v] = alt','    prev[v] = u','   end if','  end for',' end while','end procedure'],
'A* Search':['procedure A*(start, goal)',' openSet.add(start)',' while openSet is not empty','  current = node in openSet with lowest fCost','  if current == goal','    return PathFound','  end if','  for each neighbor of current','   tentative_gCost = gCost[current] + 1','   if tentative_gCost < gCost[neighbor]','    parent[neighbor] = current','    gCost[neighbor] = tentative_gCost','    fCost[neighbor] = gCost + heuristic','    openSet.add(neighbor)','   end if','  end for',' end while',' return PathNotFound','end procedure']};
let mode='sorting',algorithm='Bubble Sort',arr=[],original=[],running=false,paused=false,timer=null,sortGen=null,sortState={comp:0,access:0,active:[],sorted:[],line:1};
const rows=20,cols=32;let grid=[],startNode={r:2,c:3},endNode={r:17,c:28},pathGen=null,pathTimer=null,pathRunning=false,pathPaused=false, pathState={visited:0,cost:0,active:[],path:[],line:1,open:new Set(),visitedSet:new Set()},mouseDown=false,pathTool='start',pathRunId=0;
function resizeCanvas(){const d=devicePixelRatio||1,w=canvas.clientWidth||1000,h=canvas.clientHeight||570;canvas.width=w*d;canvas.height=h*d;ctx.setTransform(d,0,0,d,0,0);draw()};window.addEventListener('resize',resizeCanvas);

function escapeHtml(text){return text.replace(/[&<>"']/g,ch=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));}
function currentLine(){return mode==='sorting'?sortState.line:pathState.line;}
function renderPseudocode(){const lines=pseudo[algorithm]||[];$('pseudocode').innerHTML=lines.map((line,i)=>`<div class=\"code-line ${i+1===currentLine()?'active':''}\" data-line=\"${i+1}\"><span class=\"line-number\">${i+1}</span><span class=\"line-text\">${escapeHtml(line)}</span></div>`).join('');}
function highlightLine(line){if(!line)return;if(mode==='sorting')sortState.line=line;else pathState.line=line;document.querySelectorAll('.code-line').forEach(el=>el.classList.toggle('active',Number(el.dataset.line)===line));const active=document.querySelector('.code-line.active');if(active)active.scrollIntoView({block:'nearest'});}

function setAlgorithms(){const names=mode==='sorting'?sortNames:pathNames;$('algorithm').innerHTML=names.map(n=>`<option>${n}</option>`).join('');algorithm=names[0];$('algorithm-title').textContent=algorithm;renderPseudocode()}
function setMode(m){
  mode=m;
  stop();
  document.querySelectorAll('.nav-btn').forEach(b=>b.classList.toggle('active',b.dataset.mode===m));
  $('sort-controls').classList.toggle('hidden',m!=='sorting');
  $('path-controls').classList.toggle('hidden',m!=='pathfinding');
  $('sort-actions').classList.toggle('hidden',m!=='sorting');
  $('path-actions').classList.toggle('hidden',m!=='pathfinding');
  $('clear').classList.toggle('hidden',m!=='sorting');
  $('hero-title').textContent=m==='sorting'?'See sorting algorithms in motion.':'Explore how pathfinding algorithms navigate a grid.';
  $('hero-copy').textContent=m==='sorting'?'Generate an array, choose an algorithm, and watch each comparison and swap happen step by step.':'Build a maze or draw your own walls, then watch BFS, DFS, Dijkstra, or A* search for a route.';
  $('mode-pill').textContent=m.toUpperCase();
  $('legend').innerHTML=m==='sorting'
    ? '<div><i class="swatch compare"></i> Comparing</div><div><i class="swatch swap"></i> Swapping</div><div><i class="swatch sorted"></i> Sorted</div>'
    : '<div><i class="swatch start"></i> Start</div><div><i class="swatch end"></i> End</div><div><i class="swatch wall"></i> Wall</div><div><i class="swatch path"></i> Path</div><div><i class="swatch visited"></i> Visited</div><div><i class="swatch open"></i> Open Set</div><div><i class="swatch weighted"></i> Weighted</div>';
  setAlgorithms();
  if(m==='sorting') generateArray(); else generateGrid();
  if(mode==='pathfinding') syncPathControls(); else syncControls();
}
function syncControls(){
  const start=$('start'),pause=$('pause');
  if(!running){start.textContent='Start';start.disabled=false;pause.textContent='⏸ Pause';pause.disabled=true;}
  else if(paused){start.textContent='▶ Start / Resume';start.disabled=false;pause.textContent='▶ Resume';pause.disabled=false;}
  else{start.textContent='Running…';start.disabled=true;pause.textContent='⏸ Pause';pause.disabled=false;}
}

function generateArray(){stop();const n=+$('size').value;arr=Array.from({length:n},()=>Math.floor(Math.random()*390)+15);original=[...arr];sortState={comp:0,access:0,active:[],sorted:[],line:1};renderPseudocode();syncControls();updateStats();draw()}
function resetSort(){stop();arr=[...original];sortState={comp:0,access:0,active:[],sorted:[],line:1};renderPseudocode();syncControls();updateStats();draw()}
function* bubble(){let a=arr,n=a.length;for(let i=0;i<n-1;i++){let swapped=false;for(let j=0;j<n-i-1;j++){yield {type:'compare',i:j,j:j+1};sortState.comp++;sortState.access+=2;if(a[j]>a[j+1]){[a[j],a[j+1]]=[a[j+1],a[j]];sortState.access+=2;yield {type:'swap',i:j,j:j+1};swapped=true}}sortState.sorted.push(n-i-1);if(!swapped)break}return}
function* selection(){let a=arr,n=a.length;for(let i=0;i<n;i++){let min=i;for(let j=i+1;j<n;j++){yield {type:'compare',i:j,j:min};sortState.comp++;sortState.access+=2;if(a[j]<a[min])min=j}if(min!==i){[a[i],a[min]]=[a[min],a[i]];sortState.access+=4;yield {type:'swap',i:i,j:min}}sortState.sorted.push(i)}}
function* insertion(){let a=arr;for(let i=1;i<a.length;i++){let j=i;while(j>0){yield {type:'compare',i:j,j:j-1};sortState.comp++;sortState.access+=2;if(a[j]>=a[j-1])break;[a[j],a[j-1]]=[a[j-1],a[j]];sortState.access+=2;yield {type:'swap',i:j,j:j-1};j--}sortState.sorted.push(i)}}
function* merge(){let a=arr,n=a.length;function* ms(l,r){if(r-l<=1)return;let m=(l+r)>>1;yield* ms(l,m);yield* ms(m,r);let tmp=[],i=l,j=m;while(i<m||j<r){if(j>=r||(i<m&&a[i]<=a[j]))tmp.push(a[i++]);else tmp.push(a[j++]);sortState.comp++;sortState.access+=2}for(let k=0;k<tmp.length;k++){a[l+k]=tmp[k];sortState.access++;yield {type:'write',i:l+k}}}yield* ms(0,n);sortState.sorted=Array.from({length:n},(_,i)=>i)}
function* quick(){let a=arr;function* q(l,r){if(l>=r)return;let p=a[r],i=l;for(let j=l;j<r;j++){yield {type:'compare',i:j,j:r};sortState.comp++;sortState.access+=2;if(a[j]<p){[a[i],a[j]]=[a[j],a[i]];sortState.access+=4;yield {type:'swap',i:i,j:j};i++}}[a[i],a[r]]=[a[r],a[i]];sortState.access+=4;yield {type:'swap',i:i,j:r,line:16};yield* q(l,i-1);yield* q(i+1,r)}yield* q(0,a.length-1);sortState.sorted=Array.from({length:a.length},(_,i)=>i)}
function lineForSortEvent(ev){
  if(ev&&ev.line)return ev.line;
  const map={
    'Bubble Sort':{compare:6,swap:7},
    'Selection Sort':{compare:5,swap:10},
    'Insertion Sort':{compare:5,swap:6},
    'Merge Sort':{compare:6,write:6},
    'Quick Sort':{compare:11,swap:13}
  };
  return map[algorithm]?.[ev?.type]||1;
}

function startSort(){
  if(running&&paused){paused=false;$('progress').textContent='Running…';syncControls();stepSort();return;}
  if(running)return;
  running=true;paused=false;
  if(!sortGen)sortGen={"Bubble Sort":bubble,"Selection Sort":selection,"Insertion Sort":insertion,"Merge Sort":merge,"Quick Sort":quick}[algorithm]();
  $('progress').textContent='Running…';syncControls();stepSort();
}
function stepSort(){
  if(!running||paused)return;
  const res=sortGen.next();
  if(res.done){running=false;paused=false;sortState.sorted=Array.from({length:arr.length},(_,i)=>i);sortState.active=[];sortState.line=pseudo[algorithm].length;$('progress').textContent='Completed';syncControls();updateStats();draw();return;}
  const ev=res.value||{};sortState.line=lineForSortEvent(ev);highlightLine(sortState.line);
  sortState.active=ev.i!==undefined?[ev.i,ev.j].filter(x=>x!=null):[];
  updateStats();draw();timer=setTimeout(stepSort,[180,110,65,35,15][+$('speed').value-1]);
}
function pathSpeedDelay(){return [180,110,65,35,15][+$('path-speed').value-1]}
function inBounds(r,c){return r>=0&&r<rows&&c>=0&&c<cols}
function neighbors(n){
  const dirs=[[1,0],[-1,0],[0,1],[0,-1]];
  if($('diagonal').checked) dirs.push([1,1],[1,-1],[-1,1],[-1,-1]);
  return dirs.map(([dr,dc])=>({r:n.r+dr,c:n.c+dc,step:(dr&&dc)?Math.SQRT2:1}))
    .filter(x=>inBounds(x.r,x.c)&&!grid[x.r][x.c].wall);
}
function key(n){return n.r+','+n.c}
function heuristic(a,b){
  const dr=Math.abs(a.r-b.r),dc=Math.abs(a.c-b.c);
  return $('diagonal').checked ? Math.max(dr,dc)+(Math.SQRT2-1)*Math.min(dr,dc) : dr+dc;
}
function clearSearchState(){
  for(const row of grid) for(const cell of row){cell.seen=false;cell.open=false;cell.path=false;}
  pathState={visited:0,cost:0,active:[],path:[],line:1,open:new Set(),visitedSet:new Set()};
}
function reconstructPath(parent){
  const out=[];let cur=endNode, guard=0;
  while(cur && guard++<rows*cols+2){out.unshift(cur);if(key(cur)===key(startNode))break;cur=parent.get(key(cur));}
  return out;
}
function* pathAlgorithm(){
  const s={...startNode},e={...endNode};
  const parent=new Map(),dist=new Map([[key(s),0]]),open=new Map();
  const closed=new Set();
  open.set(key(s),s); grid[s.r][s.c].open=true; pathState.open.add(key(s));
  const isBfs=algorithm==='BFS', isDfs=algorithm==='DFS';
  const frontier=isBfs?[s]:isDfs?[s]:null;

  while((frontier?frontier.length:open.size)>0){
    let cur;
    if(frontier){
      cur=isBfs?frontier.shift():frontier.pop();
      grid[cur.r][cur.c].open=false;pathState.open.delete(key(cur));
      pathState.line=5; yield {type:'current',node:cur,line:5};
    }else{
      let bestKey=null,best=Infinity;
      for(const [k,n] of open){
        const g=dist.get(k)??Infinity;
        const score=algorithm==='A*'?g+heuristic(n,e):g;
        if(score<best){best=score;bestKey=k;}
      }
      cur=open.get(bestKey);
      open.delete(bestKey);grid[cur.r][cur.c].open=false;pathState.open.delete(bestKey);
      yield {type:'current',node:cur,line:4};
    }

    const ck=key(cur);
    if(closed.has(ck)) continue;
    closed.add(ck);grid[cur.r][cur.c].seen=true;pathState.visitedSet.add(ck);pathState.visited++;
    if(ck===key(e)){
      pathState.line=algorithm==='A*'?5:(algorithm==='Dijkstra'?6:algorithm==='BFS'?7:8);
      yield {type:'found',node:cur,line:pathState.line};
      const p=reconstructPath(parent);pathState.cost=pathCost(p);
      for(const node of p){grid[node.r][node.c].path=true;pathState.path.push(node);yield {type:'path',node,line:algorithm==='A*'?10:algorithm==='Dijkstra'?9:12};}
      return;
    }

    const nbs=neighbors(cur);
    for(const nb of nbs){
      const nk=key(nb);
      if(closed.has(nk)) continue;
      const step=nb.step*(grid[nb.r][nb.c].weight||1);
      const nd=(dist.get(ck)??0)+step;
      if(isBfs||isDfs){
        if(dist.has(nk)) continue;
        dist.set(nk,nd);parent.set(nk,cur);
        frontier.push(nb);grid[nb.r][nb.c].open=true;pathState.open.add(nk);
        yield {type:'open',node:nb,line:algorithm==='BFS'?10:11};
      }else if(nd<(dist.get(nk)??Infinity)){
        dist.set(nk,nd);parent.set(nk,cur);
        if(!open.has(nk)){open.set(nk,nb);grid[nb.r][nb.c].open=true;pathState.open.add(nk);}
        yield {type:'open',node:nb,line:algorithm==='A*'?8:7};
      }
    }
  }
  pathState.line=pseudo[algorithm].length;
}
function pathCost(path){
  let total=0;
  for(let i=1;i<path.length;i++){
    const a=path[i-1],b=path[i],diag=a.r!==b.r&&a.c!==b.c;
    total+=(diag?Math.SQRT2:1)*(grid[b.r][b.c].weight||1);
  }
  return path.length?Math.round(total*100)/100:0;
}
function stopPathAlgorithm(){
  if(pathTimer) clearTimeout(pathTimer);
  pathTimer=null;
  pathRunning=false;
  pathPaused=false;
  pathGen=null;
  pathRunId++;
}
function startPath(){
  if(pathRunning&&pathPaused){
    pathPaused=false;
    $('progress').textContent='Running…';
    syncPathControls();
    stepPath();
    return;
  }
  if(pathRunning)return;
  clearSearchState();
  pathRunId++;
  pathRunning=true;
  pathPaused=false;
  pathGen=pathAlgorithm();
  $('progress').textContent='Running…';
  syncPathControls();
  stepPath();
}
function stepPath(){
  if(!pathRunning||pathPaused||!pathGen)return;
  const run=pathRunId,res=pathGen.next();
  if(res.done){
    pathRunning=false;
    pathPaused=false;
    pathGen=null;
    pathTimer=null;
    pathState.active=[];
    pathState.line=pseudo[algorithm].length;
    $('progress').textContent=pathState.path.length?'Completed':'No path found';
    syncPathControls();updateStats();draw();return;
  }
  const ev=res.value||{};
  pathState.active=ev.node?[ev.node]:[];
  if(ev.line) highlightLine(ev.line);
  updateStats();draw();
  pathTimer=setTimeout(()=>{
    pathTimer=null;
    if(run===pathRunId&&pathRunning&&!pathPaused) stepPath();
  },pathSpeedDelay());
}
function syncPathControls(){
  const start=$('path-start'),pause=$('path-pause');
  if(!pathRunning){
    start.textContent='Start';start.disabled=false;
    pause.textContent='⏸ Pause';pause.disabled=true;
  }else if(pathPaused){
    start.textContent='▶ Start / Resume';start.disabled=false;
    pause.textContent='▶ Resume';pause.disabled=false;
  }else{
    start.textContent='Running…';start.disabled=true;
    pause.textContent='⏸ Pause';pause.disabled=false;
  }
}
function clearPath(){
  if(mode!=='pathfinding')return;
  stopPathAlgorithm();
  clearSearchState();
  $('progress').textContent='Ready';
  highlightLine(1);
  syncPathControls();
  updateStats();draw();
}
function resetPath(){
  if(mode!=='pathfinding')return;
  stopPathAlgorithm();
  generateGrid();
  $('progress').textContent='Ready';
  syncPathControls();
  updateStats();draw();
}
function maze(){
  if(mode!=='pathfinding')return;
  stopPathAlgorithm();
  clearSearchState();
  for(let r=0;r<rows;r++)for(let c=0;c<cols;c++){
    const border=r===0||c===0||r===rows-1||c===cols-1;
    grid[r][c].wall=border||Math.random()<.22;
    grid[r][c].weight=1;
  }
  grid[startNode.r][startNode.c].wall=false;
  grid[endNode.r][endNode.c].wall=false;
  $('progress').textContent='Ready';
  highlightLine(1);
  syncPathControls();
  updateStats();draw();
}
function generateGrid(){
  // Do not touch sorting state when rebuilding the pathfinding grid.
  stopPathAlgorithm();
  grid=Array.from({length:rows},()=>Array.from({length:cols},()=>({wall:false,weight:1,seen:false,open:false,path:false})));
  startNode={r:2,c:3};endNode={r:17,c:28};pathTool='start';
  pathState={visited:0,cost:0,active:[],path:[],line:1,open:new Set(),visitedSet:new Set()};
  setPathTool('start',false);renderPseudocode();updateStats();draw();
}
function setPathTool(tool,clearFirst=true){
  if(mode!=='pathfinding')return;
  if(clearFirst) clearPath();
  pathTool=tool;
  document.querySelectorAll('.tool-btn').forEach(b=>b.classList.toggle('active',b.id===`tool-${tool}`));
}
function editCell(r,c){
  if(!inBounds(r,c)||pathRunning)return;
  const cell=grid[r][c];
  if(pathTool==='start'){
    if(key({r,c})===key(endNode))return;
    startNode={r,c};cell.wall=false;cell.weight=1;
  }else if(pathTool==='end'){
    if(key({r,c})===key(startNode))return;
    endNode={r,c};cell.wall=false;cell.weight=1;
  }else if(pathTool==='wall'){
    if(key({r,c})===key(startNode)||key({r,c})===key(endNode))return;
    cell.wall=!cell.wall; if(cell.wall)cell.weight=1;
  }else if(pathTool==='weight'){
    if(key({r,c})===key(startNode)||key({r,c})===key(endNode)||cell.wall)return;
    cell.weight=cell.weight>1?1:5;
  }
  draw();
}
function stop(){
  if(timer)clearTimeout(timer);
  timer=null;
  running=false;
  paused=false;
  sortGen=null;
  stopPathAlgorithm();
  if(mode==='pathfinding') syncPathControls(); else syncControls();
}
function updateStats(){
  if(mode==='sorting'){
    $('stat-a-label').textContent='Comparisons';$('stat-a').textContent=sortState.comp;
    $('stat-b-label').textContent='Array accesses';$('stat-b').textContent=sortState.access;
    $('stat-c').textContent=paused?'Paused':running?'Running':($('progress').textContent==='Completed'?'Completed':'Ready');
  }else{
    $('stat-a-label').textContent='Nodes visited';$('stat-a').textContent=pathState.visited;
    $('stat-b-label').textContent='Path cost';$('stat-b').textContent=pathState.cost;
    $('stat-c').textContent=pathPaused?'Paused':pathRunning?'Searching':($('progress').textContent==='Completed'?'Completed':$('progress').textContent==='No path found'?'No path':'Ready');
  }
}
function draw(){
  const w=canvas.clientWidth||1000,h=canvas.clientHeight||570;
  ctx.clearRect(0,0,w,h);ctx.fillStyle='#0d1016';ctx.fillRect(0,0,w,h);
  if(mode==='sorting')drawSort(w,h);else drawGrid(w,h);
}
function drawSort(w,h){
  if(!arr.length)return;
  const max=Math.max(...arr,1),gap=Math.max(1,w/arr.length*.18),bw=w/arr.length-gap;
  for(let i=0;i<arr.length;i++){
    const bh=(arr[i]/max)*(h*.82),x=i*w/arr.length+gap/2,y=h-35-bh;
    let color='#8b5cf6';
    if(sortState.sorted.includes(i))color='#34d399';
    if(sortState.active.includes(i))color=sortState.active.length>1&&sortState.active[0]!==sortState.active[1]?'#facc15':'#f87171';
    ctx.fillStyle=color;ctx.fillRect(x,y,Math.max(1,bw),bh);
  }
}
function drawGrid(w,h){
  const cw=w/cols,ch=h/rows,pathSet=new Set(pathState.path.map(key));
  const activeSet=new Set(pathState.active.map(key));
  const openSet=pathState.open||new Set();
  for(let r=0;r<rows;r++)for(let c=0;c<cols;c++){
    const cell=grid[r]?.[c]||{},k=r+','+c;
    let color='#171b24';
    if(cell.wall)color='#555b68';
    else if(pathSet.has(k)||cell.path)color='#34d399';
    else if(activeSet.has(k))color='#f97316';
    else if(openSet.has(k)||cell.open)color='#f59e0b';
    else if(cell.seen||pathState.visitedSet?.has(k))color='#8b5cf6';
    else if(cell.weight>1)color='#8a6d1d';
    ctx.fillStyle=color;ctx.fillRect(c*cw+1,r*ch+1,cw-2,ch-2);
    if(cell.weight>1&&!cell.wall){
      ctx.fillStyle='#facc15';ctx.beginPath();ctx.arc(c*cw+cw/2,r*ch+ch/2,Math.min(cw,ch)*.16,0,Math.PI*2);ctx.fill();
      ctx.fillStyle='#fff7b2';ctx.font=`${Math.max(9,Math.min(cw,ch)*.34)}px sans-serif`;ctx.textAlign='center';ctx.textBaseline='middle';ctx.fillText('5',c*cw+cw/2,r*ch+ch/2);
    }
  }
  ctx.textAlign='left';ctx.textBaseline='alphabetic';
  ctx.fillStyle='#22c55e';ctx.fillRect(startNode.c*cw+2,startNode.r*ch+2,cw-4,ch-4);
  ctx.fillStyle='#ef4444';ctx.fillRect(endNode.c*cw+2,endNode.r*ch+2,cw-4,ch-4);
  ctx.fillStyle='white';ctx.font=`bold ${Math.max(9,Math.min(cw,ch)*.38)}px sans-serif`;ctx.textAlign='center';ctx.textBaseline='middle';
  ctx.fillText('S',startNode.c*cw+cw/2,startNode.r*ch+ch/2);ctx.fillText('E',endNode.c*cw+cw/2,endNode.r*ch+ch/2);
  ctx.textAlign='left';ctx.textBaseline='alphabetic';
}
function cellFromPointer(e){
  const rect=canvas.getBoundingClientRect();
  const c=Math.floor((e.clientX-rect.left)/rect.width*cols),r=Math.floor((e.clientY-rect.top)/rect.height*rows);
  return inBounds(r,c)?{r,c}:null;
}
canvas.addEventListener('pointerdown',e=>{
  if(mode!=='pathfinding'||pathRunning)return;
  mouseDown=true;canvas.setPointerCapture?.(e.pointerId);
  const n=cellFromPointer(e);if(n)editCell(n.r,n.c);
});
canvas.addEventListener('pointermove',e=>{
  if(mode!=='pathfinding'||!mouseDown||pathRunning)return;
  if(pathTool==='wall'||pathTool==='weight'){const n=cellFromPointer(e);if(n)editCell(n.r,n.c);}
});
canvas.addEventListener('pointerup',e=>{mouseDown=false;try{canvas.releasePointerCapture?.(e.pointerId)}catch(_){}});
canvas.addEventListener('pointercancel',()=>mouseDown=false);

$('algorithm').addEventListener('change',e=>{
  algorithm=e.target.value;$('algorithm-title').textContent=algorithm;renderPseudocode();
  if(mode==='sorting')resetSort();else clearPath();
});
$('size').addEventListener('input',()=>{$('size-value').textContent=$('size').value;generateArray()});
$('speed').addEventListener('input',()=>{$('speed-value').textContent=['Slow','Slow','Medium','Fast','Very fast'][+$('speed').value-1]});
$('path-speed').addEventListener('input',()=>{$('path-speed-value').textContent=['Slow','Slow','Medium','Fast','Very fast'][+$('path-speed').value-1]});
$('diagonal').addEventListener('change',()=>{if(mode==='pathfinding')clearPath()});

$('start').onclick=()=>startSort();
$('pause').onclick=()=>{
  if(!running)return;
  if(paused){paused=false;$('progress').textContent='Running…';syncControls();stepSort();}
  else{paused=true;if(timer)clearTimeout(timer);timer=null;$('progress').textContent='Paused';syncControls();updateStats();}
};
$('reset').onclick=resetSort;
$('generate').onclick=generateArray;
$('clear').onclick=resetSort;

$('path-start').onclick=startPath;
$('path-pause').onclick=()=>{
  if(!pathRunning)return;
  if(pathPaused){
    pathPaused=false;
    $('progress').textContent='Running…';
    syncPathControls();
    stepPath();
  }else{
    pathPaused=true;
    if(pathTimer)clearTimeout(pathTimer);
    pathTimer=null;
    $('progress').textContent='Paused';
    syncPathControls();
    updateStats();
  }
};
$('path-clear').onclick=clearPath;
$('path-reset').onclick=resetPath;
$('maze').onclick=maze;
$('tool-start').onclick=()=>setPathTool('start',true);
$('tool-end').onclick=()=>setPathTool('end',true);
$('tool-wall').onclick=()=>setPathTool('wall',true);
$('tool-weight').onclick=()=>setPathTool('weight',true);

document.querySelectorAll('.nav-btn').forEach(b=>b.onclick=()=>setMode(b.dataset.mode));
setMode('sorting');
$('speed-value').textContent='Medium';
$('path-speed-value').textContent='Medium';
resizeCanvas();
syncControls();


// First-visit instructions. This only controls the onboarding UI; algorithm logic is unchanged.
(function initWelcomeGuide(){
  const modal=$('welcome-modal');
  const close=()=>{
    modal.classList.add('hidden');
    if($('welcome-dont-show').checked) localStorage.setItem('pathPivotWelcomeSeen','1');
  };
  const open=()=>{
    $('welcome-dont-show').checked=false;
    modal.classList.remove('hidden');
  };
  $('help-btn').addEventListener('click',open);
  $('welcome-close').addEventListener('click',close);
  $('welcome-start').addEventListener('click',close);
  modal.querySelector('[data-close-welcome]').addEventListener('click',close);
  document.addEventListener('keydown',e=>{if(e.key==='Escape'&&!modal.classList.contains('hidden'))close();});
  if(!localStorage.getItem('pathPivotWelcomeSeen')) modal.classList.remove('hidden');
})();
