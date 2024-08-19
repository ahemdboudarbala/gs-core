#ifndef NETSTREAMCONSTANTS_HPP
#define NETSTREAMCONSTANTS_HPP

class NetStreamConstants {
public:
    static const int EVENT_GETVERSION = 0x00;
    static const int EVENT_START = 0x01;
    static const int EVENT_END = 0x02;

    static const int EVENT_ADD_NODE = 0x10;
    static const int EVENT_DEL_NODE = 0x11;
    static const int EVENT_ADD_EDGE = 0x12;
    static const int EVENT_DEL_EDGE = 0x13;
    static const int EVENT_STEP = 0x14;
    static const int EVENT_CLEARED = 0x15;

    static const int EVENT_ADD_GRAPH_ATTR = 0x16;
    static const int EVENT_CHG_GRAPH_ATTR = 0x17;
    static const int EVENT_DEL_GRAPH_ATTR = 0x18;

    static const int EVENT_ADD_NODE_ATTR = 0x19;
    static const int EVENT_CHG_NODE_ATTR = 0x1A;
    static const int EVENT_DEL_NODE_ATTR = 0x1B;

    static const int EVENT_ADD_EDGE_ATTR = 0x1C;
    static const int EVENT_CHG_EDGE_ATTR = 0x1D;
    static const int EVENT_DEL_EDGE_ATTR = 0x1E;

    static const int TYPE_UNKNOWN = 0x00;
    static const int TYPE_BOOLEAN = 0x50;
    static const int TYPE_BOOLEAN_ARRAY = 0x51;
    static const int TYPE_BYTE = 0x52;
    static const int TYPE_BYTE_ARRAY = 0x53;
    static const int TYPE_SHORT = 0x54;
    static const int TYPE_SHORT_ARRAY = 0x55;
    static const int TYPE_INT = 0x56;
    static const int TYPE_INT_ARRAY = 0x57;
    static const int TYPE_LONG = 0x58;
    static const int TYPE_LONG_ARRAY = 0x59;
    static const int TYPE_FLOAT = 0x5A;
    static const int TYPE_FLOAT_ARRAY = 0x5B;
    static const int TYPE_DOUBLE = 0x5C;
    static const int TYPE_DOUBLE_ARRAY = 0x5D;
    static const int TYPE_STRING = 0x5E;
    static const int TYPE_STRING_ARRAY = 0x62;
    static const int TYPE_RAW = 0x5F;
    static const int TYPE_ARRAY = 0x60;
    static const int TYPE_NULL = 0x61;

    static const int COMMAND = 0x70;
};

#endif // NETSTREAMCONSTANTS_HPP
