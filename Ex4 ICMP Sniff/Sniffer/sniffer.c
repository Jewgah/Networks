// ICMP Packet Sniffer - Ex4 Networks - Ariel University 2021
// Jordan Perez & Nathanael Benichou

#include<pcap.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>//icmp header
#include<netinet/ip.h>//ip header


void process(u_char *, const struct pcap_pkthdr *, const u_char *);


struct sockaddr_in source,dest;
int icmp=0;

int main()
{
    pcap_if_t *alldevices , *device;
    pcap_t *handle;

    char error_buffer[100] , *device_name , devs[100][100];
    int counter = 1 , n;

    //Get a list of all available devices
    printf("\nFinding devices ...");
    if( pcap_findalldevs( &alldevices , error_buffer) ) //function to get all the available devices
    {
        printf("Error : %s" , error_buffer);
        exit(1);
    }
    printf("Success !");

    //Print available devices
    printf("\nAvailable Devices are :\n");
    for(device = alldevices ; device != NULL ; device = (*device).next)
    {
        printf("%d. %s - %s\n" , counter , (*device).name , (*device).description);
        if((*device).name != NULL)
        {
            strcpy(devs[counter] , (*device).name);
        }
        counter++;
    }

    //Ask which device to sniff
    printf("\nWhich device would you want to sniff ? ");
    scanf("%d" , &n);
    device_name = devs[n];

    //Open it
    printf("Opening device %s for sniffing ... " , device_name);
    handle = pcap_open_live(device_name , 65536 , 1 , 0 , error_buffer); //get the device to sniff

    if (handle == NULL)
    {
        printf( "Error: Cannot open device %s : %s\n" , device_name , error_buffer);
        exit(1);
    }
    printf("Done\n");


    //Put the device in sniff loop
    pcap_loop(handle , -1 , process , NULL);

    return 0;
}

void process(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    int size = (*header).len;

    //Get only the IP Header of current packet (and not the Ethernet header)
    struct iphdr *ipheader = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    if ((*ipheader).protocol==1){
        ++icmp;

        // print packets:
        unsigned short iphdrlen;

        struct iphdr *ipheader = (struct iphdr *)(buffer  + sizeof(struct ethhdr));
        iphdrlen = (*ipheader).ihl * 4;

        struct icmphdr *icmpheader = (struct icmphdr *)(buffer + iphdrlen  + sizeof(struct ethhdr));

        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = (*ipheader).saddr;

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = (*ipheader).daddr;
	printf("\n");
        printf( "   #Source IP        : %s\n" , inet_ntoa(source.sin_addr) );
        printf( "   #Destination IP   : %s\n" , inet_ntoa(dest.sin_addr) );
        printf( "   #Code : %d\n",(unsigned int)((*icmpheader).code));
        printf( "   #Type : %d\n",(unsigned int)((*icmpheader).type));
        printf("______________________________________________________\n" );
    }

    printf("Number of Packet ICMP : %d \r",icmp ); //print number of icmp packets
}
