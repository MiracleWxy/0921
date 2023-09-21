#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <math.h>

// 结构体用于存储节点信息
typedef struct {
    unsigned int id;
    double lat;
    double lon;
} NodeInfo;

int main() {
    // 初始化libxml2库
    xmlInitParser();

    // 读取XML文件
    xmlDocPtr doc = xmlReadFile("map.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to open the XML file.\n");
        return 1;
    }

    // 获取根节点
    xmlNodePtr root = xmlDocGetRootElement(doc);

    // 遍历XML树
    NodeInfo nodes[12000]; // 假设最多有100个匹配的节点
    int nodeCount = 0;

    for (xmlNodePtr node = root; node != NULL; node = node->next)  {
        if (node->type == XML_ELEMENT_NODE) {
            // 如果是way节点并且id为指定
            if (xmlStrcmp(node->name, (const xmlChar *)"way") == 0) {
                xmlChar *wayIdStr = xmlGetProp(node, (const xmlChar *)"id");
                if (wayIdStr != NULL && xmlStrcmp(wayIdStr, (const xmlChar *)"28842841") == 0) {
			printf("2");
                    // 在way节点下查找nd节点
                    for (xmlNodePtr ndNode = node->children; ndNode != NULL; ndNode = ndNode->next) {
                        if (xmlStrcmp(ndNode->name, (const xmlChar *)"nd") == 0) {
                            xmlChar *refStr = xmlGetProp(ndNode, (const xmlChar *)"ref");
                            if (refStr != NULL) {
                                unsigned int refId = strtoul((char *)refStr, NULL, 10);
				printf("%d\n",refId);
                                // 在整个文档中查找匹配的node节点
                                for (xmlNodePtr node2 = root; node2 != NULL; node2 = node2->next) {
                                    if (node2->type == XML_ELEMENT_NODE && xmlStrcmp(node2->name, (const xmlChar *)"node") == 0) {
                                        xmlChar *nodeIdStr = xmlGetProp(node2, (const xmlChar *)"id");
                                        xmlChar *latStr = xmlGetProp(node2, (const xmlChar *)"lat");
                                        xmlChar *lonStr = xmlGetProp(node2, (const xmlChar *)"lon");
                                        if (nodeIdStr != NULL && latStr != NULL && lonStr != NULL) {
                                            unsigned int nodeId = strtoul((char *)nodeIdStr, NULL, 10);
                                            if (nodeId == refId) {
                                                double lat = atof((char *)latStr);
                                                double lon = atof((char *)lonStr);
                                                nodes[nodeCount].id = nodeId;
                                                nodes[nodeCount].lat = lat;
                                                nodes[nodeCount].lon = lon;
                                                nodeCount++;
                                                xmlFree(nodeIdStr);
                                                xmlFree(latStr);
                                                xmlFree(lonStr);
                                            }
                                        }
                                    }
                                }
                                xmlFree(refStr);
                            }
                        }
                    }
                }
                xmlFree(wayIdStr);
            }
        }
    }

    // 打印找到的节点
    for (int i = 0; i < nodeCount; i++) {
        printf("Node %u: lat=%.6lf, lon=%.6lf\n", nodes[i].id, nodes[i].lat, nodes[i].lon);
    }

    // 计算距离并打印
    for (int i = 0; i < nodeCount - 1; i++) {
        double lat1 = nodes[i].lat;
        double lon1 = nodes[i].lon;
        double lat2 = nodes[i + 1].lat;
        double lon2 = nodes[i + 1].lon;

        // 使用Haversine公式计算距离
        double dLat = (lat2 - lat1) * M_PI / 180.0;
        double dLon = (lon2 - lon1) * M_PI / 180.0;
        double a = sin(dLat / 2) * sin(dLat / 2) +
                   cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                   sin(dLon / 2) * sin(dLon / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double distance = 6371 * c; // 地球半径为6371公里

        printf("Distance between Node %u and Node %u: %.2lf km\n", nodes[i].id, nodes[i + 1].id, distance);
    }

    // 释放资源
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}

