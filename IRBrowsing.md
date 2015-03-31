# Dumping IR to file #
### ICC ###

To dump IR in ICC use the following command line:
```
icc <your src-file> -mP2OPT_il0_print=<num>
```
where _num_ is a phase number.<br>
<b>Stderr</b> is used as the output stream.<br>
<br>
If <i>num</i> = -1 then the dump will be printed after each phase.<br>
Phase numbering isn't successive, the numbers of phases seem to be chosen at random.<br>
<br>
Numbers of the most useful phases:<br>
<ul><li>6    - after building CFG<br>
</li><li>2510 - after some optimizations.<br>
<br>
<h3>GCC</h3>
ShowGraph supports GCC 4.x and doesn't support GCC 3 dumps. To dump IR in GCC use the following command line:<br>
<pre><code>gcc &lt;your src-file&gt; -df<br>
</code></pre>
The output will be in src-file.cfg</li></ul>

<h1>Browsing IR</h1>
When IR is dumped you can open dumps file through the <code>File</code> menu. ShowGraph will parse the file and search for routine names. If there are multiple routines in file it will ask you to select routine of interest and then parse it.<br>
<br>
After dump is parsed ShowGraph performs automatic layout and you can see control flow graph in main window. It can be zoomed and panned.<br>
<br>
Double click on a node will show its textual representation in !dockable window. Textual representation has !clickable node numbers so that user could browse it like a set of hyperlinked pages.<br>
<br>
Note that in IR browsing mode double click behaves differently from editor mode. However if you want you can enable edition through context menu which is shown by right-clicking on graph background.<br>
<br>
<img src='http://i43.tinypic.com/34isu3t.jpg' />