#include "VerboseSink.hpp"

VerboseSink::VerboseSink() : out(std::cout) {
    setPrefix("");
    setSuffix("");

    formats[EventType::ADD_NODE] = "%prefix%[%sourceId%:%timeId%] add node \"%nodeId%\"%suffix%";
    formats[EventType::ADD_NODE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set node \"%nodeId%\" +\"%attributeId%\"=%value%%suffix%";
    formats[EventType::SET_NODE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set node \"%nodeId%\" \"%attributeId%\"=%value%%suffix%";
    formats[EventType::DEL_NODE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set node \"%nodeId%\" -\"%attributeId%\"%suffix%";
    formats[EventType::DEL_NODE] = "%prefix%[%sourceId%:%timeId%] remove node \"%nodeId%\"%suffix%";

    formats[EventType::ADD_EDGE] = "%prefix%[%sourceId%:%timeId%] add edge \"%edgeId%\" : \"%source%\" -- \"%target%\"%suffix%";
    formats[EventType::ADD_EDGE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set edge \"%edgeId%\" +\"%attributeId%\"=%value%%suffix%";
    formats[EventType::SET_EDGE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set edge \"%edgeId%\" \"%attributeId%\"=%value%%suffix%";
    formats[EventType::DEL_EDGE_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set edge \"%edgeId%\" -\"%attributeId%\"%suffix%";
    formats[EventType::DEL_EDGE] = "%prefix%[%sourceId%:%timeId%] remove edge \"%edgeId%\"%suffix%";

    formats[EventType::ADD_GRAPH_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set +\"%attributeId%\"=%value%%suffix%";
    formats[EventType::SET_GRAPH_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set \"%attributeId%\"=%value%%suffix%";
    formats[EventType::DEL_GRAPH_ATTRIBUTE] = "%prefix%[%sourceId%:%timeId%] set -\"%attributeId%\"%suffix%";

    formats[EventType::CLEAR] = "%prefix%[%sourceId%:%timeId%] clear%suffix%";
    formats[EventType::STEP_BEGINS] = "%prefix%[%sourceId%:%timeId%] step %step% begins%suffix%";

    for (auto& pair : formats) {
        enable[pair.first] = true;
    }
}

VerboseSink::VerboseSink(std::ostream& outStream) : out(outStream) {
    VerboseSink();
}

void VerboseSink::setAutoFlush(bool on) {
    autoflush = on;
}

void VerboseSink::setEventFormat(EventType type, const std::string& format) {
    formats[type] = format;
}

void VerboseSink::setEventEnabled(EventType type, bool on) {
    enable[type] = on;
}

void VerboseSink::setElementEventEnabled(bool on) {
    setEventEnabled(EventType::ADD_EDGE_ATTRIBUTE, on);
    setEventEnabled(EventType::SET_EDGE_ATTRIBUTE, on);
    setEventEnabled(EventType::DEL_EDGE_ATTRIBUTE, on);
    setEventEnabled(EventType::ADD_NODE_ATTRIBUTE, on);
    setEventEnabled(EventType::SET_NODE_ATTRIBUTE, on);
    setEventEnabled(EventType::DEL_NODE_ATTRIBUTE, on);
    setEventEnabled(EventType::ADD_GRAPH_ATTRIBUTE, on);
    setEventEnabled(EventType::SET_GRAPH_ATTRIBUTE, on);
    setEventEnabled(EventType::DEL_GRAPH_ATTRIBUTE, on);
}

void VerboseSink::setAttributeEventEnabled(bool on) {
    setEventEnabled(EventType::ADD_EDGE, on);
    setEventEnabled(EventType::DEL_EDGE, on);
    setEventEnabled(EventType::ADD_NODE, on);
    setEventEnabled(EventType::DEL_NODE, on);
    setEventEnabled(EventType::CLEAR, on);
}

void VerboseSink::setPrefix(const std::string& pfx) {
    prefix = pfx;
}

void VerboseSink::setSuffix(const std::string& sfx) {
    suffix = sfx;
}

void VerboseSink::print(EventType type, Args& args) {
    if (!enable[type])
        return;

    std::string output = formats[type];

    for (const auto& pair : args) {
        const std::string& key = pair.first;
        const std::any& value = pair.second;
        output.replace(output.find("%" + key + "%"), key.length() + 2, toStringValue(value));
    }

    out << output << std::endl;

    if (autoflush)
        out.flush();
}

VerboseSink::Args* VerboseSink::argsPnP(Args* args) {
    if (!args) {
        if (!argsStack.empty()) {
            args = argsStack.top();
            argsStack.pop();
        } else {
            args = new Args();
        }

        (*args)["prefix"] = prefix;
        (*args)["suffix"] = suffix;

        return args;
    } else {
        args->clear();
        argsStack.push(args);
        return nullptr;
    }
}

std::string VerboseSink::toStringValue(const std::any& value) {
    if (!value.has_value()) return "<null>";

    if (value.type() == typeid(std::string)) {
        return "\"" + std::any_cast<std::string>(value) + "\"";
    } else if (value.type() == typeid(const char*)) {
        return "\"" + std::string(std::any_cast<const char*>(value)) + "\"";
    } else if (value.type() == typeid(int)) {
        return std::to_string(std::any_cast<int>(value));
    } else if (value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(value));
    } else if (value.type() == typeid(float)) {
        return std::to_string(std::any_cast<float>(value));
    }
    // Extend this to handle other types if needed.
    return "<unknown>";
}

void VerboseSink::edgeAttributeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                     const std::string& attribute, const std::any& value) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["edgeId"] = edgeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(value);

    print(EventType::ADD_EDGE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::edgeAttributeChanged(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                       const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["edgeId"] = edgeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(newValue);

    print(EventType::SET_EDGE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::edgeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId, 
                                       const std::string& attribute) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["edgeId"] = edgeId;
    (*args)["attributeId"] = attribute;

    print(EventType::DEL_EDGE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::graphAttributeAdded(const std::string& sourceId, long timeId, const std::string& attribute, 
                                      const std::any& value) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(value);

    print(EventType::ADD_GRAPH_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::graphAttributeChanged(const std::string& sourceId, long timeId, const std::string& attribute, 
                                        const std::any& oldValue, const std::any& newValue) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(newValue);

    print(EventType::SET_GRAPH_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::graphAttributeRemoved(const std::string& sourceId, long timeId, const std::string& attribute) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["attributeId"] = attribute;

    print(EventType::DEL_GRAPH_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::nodeAttributeAdded(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                     const std::string& attribute, const std::any& value) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["nodeId"] = nodeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(value);

    print(EventType::ADD_NODE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::nodeAttributeChanged(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                       const std::string& attribute, const std::any& oldValue, const std::any& newValue) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["nodeId"] = nodeId;
    (*args)["attributeId"] = attribute;
    (*args)["value"] = toStringValue(newValue);

    print(EventType::SET_NODE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::nodeAttributeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId, 
                                       const std::string& attribute) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["nodeId"] = nodeId;
    (*args)["attributeId"] = attribute;

    print(EventType::DEL_NODE_ATTRIBUTE, *args);
    argsPnP(args);
}

void VerboseSink::edgeAdded(const std::string& sourceId, long timeId, const std::string& edgeId, 
                            const std::string& fromNodeId, const std::string& toNodeId, bool directed) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["edgeId"] = edgeId;
    (*args)["source"] = fromNodeId;
    (*args)["target"] = toNodeId;

    print(EventType::ADD_EDGE, *args);
    argsPnP(args);
}

void VerboseSink::edgeRemoved(const std::string& sourceId, long timeId, const std::string& edgeId) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["edgeId"] = edgeId;

    print(EventType::DEL_EDGE, *args);
    argsPnP(args);
}

void VerboseSink::graphCleared(const std::string& sourceId, long timeId) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;

    print(EventType::CLEAR, *args);
    argsPnP(args);
}

void VerboseSink::nodeAdded(const std::string& sourceId, long timeId, const std::string& nodeId) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["nodeId"] = nodeId;

    print(EventType::ADD_NODE, *args);
    argsPnP(args);
}

void VerboseSink::nodeRemoved(const std::string& sourceId, long timeId, const std::string& nodeId) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["nodeId"] = nodeId;

    print(EventType::DEL_NODE, *args);
    argsPnP(args);
}

void VerboseSink::stepBegins(const std::string& sourceId, long timeId, double step) {
    Args* args = argsPnP(nullptr);

    (*args)["sourceId"] = sourceId;
    (*args)["timeId"] = timeId;
    (*args)["step"] = step;

    print(EventType::STEP_BEGINS, *args);
    argsPnP(args);
}
