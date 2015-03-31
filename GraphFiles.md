# Graph Description #

Here's an example of ShowGraph XML graph representation.

```
<graph>
    <style pen_style="dot" pen_width="1" shape="rounded_box" name="node 2 style"/>
    <node x="-14" y="29" pen_style="dot" fill="#bddcdc" pen_width="1" id="0" shape="ellipse" label="Start"/>
    <node x="-70.5" y="85" fill="#e2ffff" pen_width="1" id="1" label="Early Exit"/>
    <edge target="1" source="0" />
    <node x="18.5" y="85" fill="#cec97b" pen_width="1" id="10" label="Loop Head" />
    <edge target="10" source="0" />
    <node x="-5" y="136" fill="#fffbc2" pen_width="1" id="11" label="Body"/>
    <edge target="11" source="10" />
    <node x="21.5" y="189" fill="#fffbc2" pen_width="1" id="12" label="Condition" />
    <edge target="12" source="11" />
    <node x="-13" y="241" pen_style="dot" fill="#bddcdc" pen_width="1" id="14" shape="ellipse" label="Stop"/>
    <edge target="14" source="12" />
    <edge target="23" pen_width="2" source="12" pen_color="#550000" />
    <node x="73" y="137" type="edge_label" id="23" label="Backedge" />
    <edge target="24" source="1" />
    <edge target="10" pen_width="2" source="23" pen_color="#550000" />
    <node x="-48" y="137" type="edge_control" id="24" label="" />
    <edge target="25" source="24" />
    <node x="-48" y="189" type="edge_control" id="25" label="" />
    <edge target="14" source="25" />
</graph>
```

Note that colors can be defined by name ( e.g "red") or by RGB code.
XML representation also stores node coordinates and edge control nodes if you don't want them to be recalculated by automatic layout. Nodes and edges can have directly specified properties or they can have property `style`, which applies a named style to node.