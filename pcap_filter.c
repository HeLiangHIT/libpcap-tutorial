#include <stdio.h>
#include <pcap.h>

int main(int argc, char *argv[])
{
    pcap_t *handle;                /* Session handle */
    char dev[] = "lo";             /* Device to sniffer on */
    char errbuf[PCAP_ERRBUF_SIZE]; /* Error String */
    struct bpf_program fp;         /* The compliled filter expression */
    char filter_exp[] = "port 80"; /* The filter expression */
    bpf_u_int32 mask;              /* The netmask of our sniffing device */
    bpf_u_int32 net;               /* The IP of our sniffering device */

/*
    dev = pcap_lookupdev(errbuf);
    if(dev == NULL)
    {
        fprintf(stderr, "Couldn't find default device: %s\n", errbuf); 
        return(2);
    }
    printf("Device: %s\n", dev);
*/

    if(pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        fprintf(stderr, "Can't get netmask for device %s\n", dev); 
        net = 0;
        mask = 0;
    }

    handle = pcap_open_live(dev, BUFSIZ, 1, 100, errbuf);
    if(handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf); 
        return(2);
    }
    
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        fprintf(stderr, "Counldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return(2);
    }

    if(pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle)); 
        return(2);
    }

    return(0);
}
