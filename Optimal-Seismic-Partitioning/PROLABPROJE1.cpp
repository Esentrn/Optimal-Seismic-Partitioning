#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <curl/curl.h>
#include <graphics.h>

// 220201109 ESENG�L TURAN 220201102 CANAN KORKUT PROLAB PROJE1

int kontrol = 0; //Kodun �al��ma kontrol� i�in kullan�lan bir global de�i�ken

//�okgenin alan�n� Gauss alan form�l� ie hesaplama (shoelace form�l� olarak da bilinir)
//Alan (A) = 1/2 *|E(xi * yi+1 - xi+1 * yi)|
int cokgenAlani(int n, int px[], int py[]) {
    int alan = 0;
    for (int i = 0; i < n; i++) {
        px[i] = px[i] / 10;
        py[i] = py[i] / 10;
    }

    int j = n - 1;
    for (int i = 0; i < n; i++) {
        alan += (px[j] + px[i]) * (py[j] - py[i]);
        j = i;
    }

    for (int i = 0; i < n; i++) {
        px[i] = px[i] * 10;
        py[i] = py[i] * 10;
    }

    alan = abs(alan / 2);

    return alan;
}

//Noktan�n �okgenin i�inde olup olmad���n� kontrol eden fonksiyon.
int icindeMi(int x[], int y[], int n, int noktaX, int noktaY) {
    int i, j, d = 0;

    for (int i = 0; i < n; i++) { //Noktan�n �okgenin i�inde olup olmad���n� kontrol eden �okgen kenarlar�n� dola�an d�ng�

        int j = i - 1;
        if (i == 0) {
            j = n - 1;
        }

        if ((y[i] > noktaY) != (y[j] > noktaY) && (noktaX < (x[j] - x[i]) * (noktaY - y[i]) / (y[j] - y[i]) + x[i])) {
            d = !d;  // Nokta, �izgiyi kesiyorsa veya �izginin s�n�r noktas�ndan ge�iyorsa, i�inde say�l�r.
        }
    }

    return d;
}

int boyaCokgen(int x[], int y[], int n, int kare) {

    int minX = x[0];
    int maxX = x[0];
    int minY = y[0];
    int maxY = y[0];

    for (int i = 1; i < n; i++) {
        if (x[i] < minX) {                 //�okgenin s�n�rlar� hesaplan�r.
            minX = x[i];
        }
        if (x[i] > maxX) {
            maxX = x[i];
        }
        if (y[i] < minY) {
            minY = y[i];
        }
        if (y[i] > maxY) {
            maxY = y[i];
        }

    }

    int kareSayisi = 0;

    for (int i = minX; i <= maxX; i += kare) {      //X ve Y koordinatlar� aras�nda dola��r.
        for (int j = minY; j <= maxY; j += kare) {
            int icindeNoktaVar = 0;

            for (int xKonum = 0; xKonum < kare; xKonum++) {   //Her birim kareyi inceler.
                for (int yKonum = 0; yKonum < kare; yKonum++) {
                    int xGonder = i + xKonum;
                    int yGonder = j + yKonum;

                    //E�er birim karenin i�inde �okgenin bir noktas� varsa, icindeNoktaVar'� 1 yapar ve d�ng�y� k�rar.
					if (icindeMi(x, y, n, xGonder, yGonder)) {
                        icindeNoktaVar = 1;
                        break;
                    }
                }
                if (icindeNoktaVar) {
                    break;
                }
            }

            if (icindeNoktaVar) { 
                kareSayisi++;                      //E�er birim kare �okgenin i�indeyse, birim kare say�s�n� artt�r�r.
                if (kontrol == 0) {
                    bar(i, j, i + kare, j + kare); //E�er birim kare �okgenin i�indeyse, boyay� ekler.
                    setcolor(BLACK);
                    rectangle(i, j, i + kare, j + kare);
                }
                if (kontrol == 2) {
                    setcolor(BLACK);
                    rectangle(i, j, i + kare, j + kare);
                }
            }
        }
    }
    return kareSayisi;
}

void optimalCizim(int x[], int y[], int s, int kare) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");  //Grafik modunu ba�lat�r.
    setbkcolor(WHITE);
    cleardevice();			 //Pencereyi temizler.

    int k = s;

    setcolor(8);

    // Izgara �izer.
    for (int i = 1; i < 10000; i++) {
        line(i * 10, 0, i * 10, 10000);
    }

    for (int j = 1; j < 10000; j++) {
        line(0, j * 10, 10000, j * 10);
    }

    
    int baslangicNoktasiX[k / 2] = {0};  //Her noktan�n ba�lang�� noktas� olup olmad���n� izlemek i�in dizi.
    int baslangicNoktasiY[k / 2] = {0};

    for (int i = 0; i < k / 2; i++) {
        if (baslangicNoktasiX[i] == 1 && baslangicNoktasiY[i] == 1) {
            setfillstyle(SOLID_FILL, RED);
            fillellipse(x[i], y[i], 4, 4); //E�er ba�lang�� noktas� ise, o noktay� k�rm�z� bir daire �izer.
            continue;  // Bu zaten bir ba�lang�� noktas� ise atlar.
        }
        for (int j = i + 1; j < k / 2; j++) {
            if (x[i] == x[j] && y[i] == y[j]) {
                baslangicNoktasiX[j] = 1;
                baslangicNoktasiY[j] = 1;

                int n = j - i;   //�okgenin k��e say�s�
                int px[n], py[n], p[2 * n];  //�okgenin x ve y koordinatlar�n� tutacak diziler px py ve t�m koordinatlar�n� tutan p dizisi.
                for (int a = i; a < j; a++) {
                    px[a - i] = x[a];
                    py[a - i] = y[a];
                }
                for (int a = i; a <= j; a++) {
                    p[2 * (a - i)] = x[a];     //�okgenin x ve y koordinatlar�n� tek bir dizi olan p dizisine kopyalar.
                    p[2 * (a - i) + 1] = y[a];
                }

                setfillstyle(SOLID_FILL, LIGHTGRAY);
                kontrol = 0;
                boyaCokgen(px, py, n, kare);  //Optimal cizim icin g�nderilen kare de�i�kinene g�re platformlar� boyar.
                
				setfillstyle(SOLID_FILL, DARKGRAY);
                fillpoly(n, p);  //Optimal cizim icin rezerv alanlar�n� boyar.
                
				kontrol = 2;
                boyaCokgen(px, py, n, kare); //Optimal cizim icin platformlar� �izer.

            }
        }
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(x[i], y[i], 4, 4); //K��e noktalar�na siyah bir daire �izer.
        setbkcolor(BLACK);
        line(x[i], y[i], x[i + 1], y[i + 1]); //�okgeni �izer.
    }

    delay(50000000);
    closegraph();
}

// HTTP �zerinden veri �ekme i�lemi i�in kullan�lan fonksiyon
size_t veriCekme(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    char *veri = (char *)userp;
    strcat(veri, (char *)contents);
    return real_size;
}

int main() {
    CURL *curl;
    CURLcode res;
    char veri[3000] = {0};
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
    	// HTTP �zerinden veri �ekme i�lemi
        curl_easy_setopt(curl, CURLOPT_URL, "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, veriCekme);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, veri);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() basar�s�z: %s\n", curl_easy_strerror(res));
        } 
	else {
            printf("%s\n\n", veri);
            int dizi[3000];
            int k = 0, m = 0;
            int satir = 0;
            printf("Lutfen kacinci satiri istediginizi giriniz:");
            scanf("%d", &satir);
            
            char *karakter;		//Sat�r ba�lang��lar� i�in pointer.
            
            if (satir == 1) {
                karakter = strstr(veri, "1B");
            } else if (satir == 2) {
                karakter = strstr(veri, "2B");
            } else if (satir == 3) {
                karakter = strstr(veri, "3B");
            } else if (satir == 4) {
                karakter = strstr(veri, "4B");
            } else if (satir == 5) {
                karakter = strstr(veri, "5B");
            } else if (satir == 6) {
                karakter = strstr(veri, "6B");
            } else if (satir == 7) {
                karakter = strstr(veri, "7B");
            } else if (satir == 8) {
                karakter = strstr(veri, "8B");
            } else if (satir == 9) {
                karakter = strstr(veri, "9B");
            } else if (satir == 10) {
                karakter = strstr(veri, "10B");
            } else if (satir == 11) {
                karakter = strstr(veri, "11B");
            } else if (satir == 12) {
                karakter = strstr(veri, "12B");
            } else if (satir == 13) {
                karakter = strstr(veri, "13B");
            } else if (satir == 14) {
                karakter = strstr(veri, "14B");
            } else if (satir == 15) {
                karakter = strstr(veri, "15B");
            } else if (satir == 16) {
                karakter = strstr(veri, "16B");
            } else if (satir == 17) {
                karakter = strstr(veri, "17B");
            } else if (satir == 18) {
                karakter = strstr(veri, "18B");
            } else if (satir == 19) {
                karakter = strstr(veri, "19B");
            } else if (satir == 20) {
                karakter = strstr(veri, "20B");
            }

            const char *ayrac = "B(,)F";   //Ayrac'a g�re karakterleri ay�r�r.
            karakter = strtok(karakter + 2, ayrac);
            
            while (karakter != NULL && *karakter != '\n' && *karakter != '\0') {
                dizi[k] = atoi(karakter);     //�nteger de�ere �evirir.
                k++;
                karakter = strtok(NULL, ayrac);
            }
            

            int gd = DETECT, gm;
            initgraph(&gd, &gm, ""); //Grafik modunu ba�lat�r.
            setbkcolor(WHITE);
            cleardevice();       //Pencereyi temizler.

            for (int i = 0; i < k; i++) {
                dizi[i] = dizi[i] * 10;
            }

            int x[k / 2], y[k / 2];   //�okgenlerin x ve y koordinatlar� i�in diziler.
            for (int i = 0; i < k / 2; i++) {
                x[i] = dizi[i * 2];
                y[i] = dizi[i * 2 + 1];
            }

            setcolor(8);

            // Izgara �izer.
            for (int i = 1; i < 10000; i++) {
                line(i * 10, 0, i * 10, 10000);
            }

            for (int j = 1; j < 10000; j++) {
                line(0, j * 10, 10000, j * 10);
            }

            int baslangicNoktasiX[k / 2] = {0};  //Her noktan�n ba�lang�� noktas� olup olmad���n� izlemek i�in bir dizi.
            int baslangicNoktasiY[k / 2] = {0};

            int cokgenSayisi = 0;
            int cokgenAlanlari[50];
            int alan2[50];
            int alan3[50][50];  //�okgenlerin optimal �izimleri i�in platformlara g�re alanlar�n� tutan 2 boyutlu dizi.

            for (int i = 0; i < k / 2; i++) {

                if (baslangicNoktasiX[i] == 1 && baslangicNoktasiY[i] == 1) {
                    setfillstyle(SOLID_FILL, RED);
                    fillellipse(x[i], y[i], 4, 4);//E�er ba�lang�� noktas� ise, o noktay� k�rm�z� bir daire �izer.
            		continue;  // Bu zaten bir ba�lang�� noktas� ise atlar.
                }
                for (int j = i + 1; j < k / 2; j++) {
                    if (x[i] == x[j] && y[i] == y[j]) {
                        baslangicNoktasiX[j] = 1;
                        baslangicNoktasiY[j] = 1;

                        printf("\nBaslangic Noktasi olan %d. parantezdeki koordinatlar: x=%d, y=%d \n", i + 1, x[i] / 10, y[i] / 10); 

                        int n = j - i;  //�okgenin k��e say�s�
                        int px[n], py[n]; //�okgenin x ve y koordinatlar�n� tutacak diziler px py. 
                        for (int a = i; a < j; a++) {
                            px[a - i] = x[a];
                            py[a - i] = y[a];
                        }

                        setfillstyle(SOLID_FILL, LIGHTGRAY);
                        kontrol = 0;
                        alan2[cokgenSayisi] = boyaCokgen(px, py, n, 10); //boyaCokgen 1*1'e g�re boyama yapar ve alan� birim kare cinsinden d�nd�r�r,alan2 dizisi alanlar� tutar.
                        

                        cokgenSayisi++;
                        int alan = cokgenAlani(n, px, py);  //Shoelace form�l� ile alan hesaplar.(Matematiksel alan)
                        cokgenAlanlari[cokgenSayisi - 1] = alan; //Matematiksel alanlar� bir dizide tutar.

                        int b = 0;
                        for (int i = 10; i <= 160; i *= 2) {

                            kontrol = 1;
                            alan3[cokgenSayisi - 1][b] = boyaCokgen(px, py, n, i); //�okgenlerin optimal �izimleri i�in (1*1,2*2,4*4,8*8,16*16)'a g�re alanlar�n� hesaplar.
                            b++;
                        }
                    }
                }
                setfillstyle(SOLID_FILL, BLACK);
                fillellipse(x[i], y[i], 4, 4); //K��e noktalar�na siyah bir daire �izer.
                setbkcolor(BLACK);
                line(x[i], y[i], x[i + 1], y[i + 1]); //�okgeni �izer.
            }

            printf("\n\nCokgen sayisi: %d\n", cokgenSayisi);
            for (int i = 0; i < cokgenSayisi; i++) {
                printf("%d. Cokgenin matematiksel alani: %d\t\tBirim alani: %d\n", i + 1, cokgenAlanlari[i], alan2[i]); //�okgenlerin alanlar�n� yazd�r�r.
            }

            int tahminiRezerv[100] = {0}, gercekRezerv[100] = {0};
            int toplamtahRezerv = 0, toplamgerRezerv = 0, toplam = 0;

            for (int i = 0; i < cokgenSayisi; i++) {
                toplam += alan2[i];
                tahminiRezerv[i] = alan2[i] * 10;
                gercekRezerv[i] = cokgenAlanlari[i] * 10;  //�okgenlerin rezervleri hesaplan�r.
            }

            printf("\n");

           

	    
	    for (int i = 0; i < cokgenSayisi; i++) {
	        printf("%d. Cokgenin matematiksel rezervi: %d\t\tBirim tahmini rezerv: %d\n", i+1, gercekRezerv[i],tahminiRezerv[i]); // �okgenlerin rezervlerini yazd�r�r.
	    }
	    
	    for (int i = 0; i < cokgenSayisi; i++) {
	    	toplamtahRezerv+=tahminiRezerv[i];
	    	toplamgerRezerv+=gercekRezerv[i];  //Toplam rezerv hesaplan�r.
	    }
	
		printf("\n\t\t\t\t\t\tToplam birim kare(1*1):%d",toplam);
	    printf("\nToplam matematiksel rezervi: %d\t\tToplam birim kareye gore tahmini rezerv: %d\n", toplamgerRezerv,toplamtahRezerv); // �okgenlerin toplam rezervini yazd�r�r.
	    printf("\n------------------------------------------------------------------------------------------------------------\n");
	   
        delay(5000); 
        closegraph(); 
        
        
        
        int sondaj,platform,toplamSondaj[5]={0},toplamPlatform[5]={0},toplamSondajsayisi[5]={0};
		int maaliyet[5]={0},toplamBirimkare[5]={0};
		
        
        printf("\n\nLutfen birim sondaj maaliyeti giriniz(1-10):");
        scanf("%d",&sondaj);
        printf("Lutfen platform maaliyeti giriniz(1-100):");
        scanf("%d",&platform);
        
        for (int i = 0; i < cokgenSayisi; i++) {
	    	  
			for(int j=0;j<5;j++){    	
				toplamBirimkare[j]+=alan3[i][j]; //�okgenlerin platformlara (1*1,2*2,4*4,8*8,16*16) g�re alanlar�n� toplay�p diziye aktar�r.

			} 
	    }
        
        printf("\n\nToplam platform sayisi(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	    for(int j=0;j<5;j++){    	
				
			printf("%d\t",toplamBirimkare[j]); //Toplam platform say�lar� karesel alanlara g�re (1*1,2*2,4*4,8*8,16*16)
		} 
	    
	   
	   int kare=1;
       
	   for(int j=0;j<5;j++){    	
			toplamPlatform[j]+=toplamBirimkare[j]*platform;  //Toplam platform maaliyeti platformlara g�re hesaplan�r.(1*1,2*2,4*4,8*8,16*16)
			toplamSondajsayisi[j]=toplamBirimkare[j]*kare;   //Toplam sondaj say�lar� platformlara g�re hesaplan�r.(1*1,2*2,4*4,8*8,16*16)
			toplamSondaj[j]+=toplamBirimkare[j]*kare*sondaj; //Toplam sondaj maaliyeti platformlara g�re hesaplan�r.(1*1,2*2,4*4,8*8,16*16)
			maaliyet[j]=toplamPlatform[j]+toplamSondaj[j];   //Toplam maaliyet platformlara g�re hesaplan�r.(1*1,2*2,4*4,8*8,16*16)
			kare*=4;
	   }
	    
	   printf("\n---------------------------------------------------------------------------------------------");
	   printf("\nToplam sondaj sayisi(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	   for(int j=0;j<5;j++){    	
				
			printf("%d\t",toplamSondajsayisi[j]);
	   } 	
	   printf("\n---------------------------------------------------------------------------------------------");
	   printf("\nToplam Platform Maaliyetleri(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	   for(int j=0;j<5;j++){	
			printf("%d\t",toplamPlatform[j]);
	   }
	   printf("\n---------------------------------------------------------------------------------------------");
	   printf("\nToplam Sondaj Maaliyetleri(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	   for(int j=0;j<5;j++){	
			printf("%d\t",toplamSondaj[j]);
	   }
	   printf("\n---------------------------------------------------------------------------------------------");
	   printf("\nToplam Maaliyetler(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	   for(int j=0;j<5;j++){	
			printf("%d\t",maaliyet[j]);
	   } 
	   printf("\n---------------------------------------------------------------------------------------------");
	   
	   int kucuk=maaliyet[0];
	   int kucukindis=0;
	   int j;
	   
	   for(j=1;j<5;j++) {
			if(maaliyet[j]<kucuk){
				kucuk=maaliyet[j];  //En k���k maaliyet bulunur.
				kucukindis=j;
			}
	   }
	   

		if(kucukindis==0)
	   		kare=10;
    	else if(kucukindis==1)
	   		kare=20;
	   	else if(kucukindis==2)    //En k���k maaliyete g�re optimal �izim i�in platform boyutlar�(karesel alan) belirlenir.
	   		kare=40;
	   	else if(kucukindis==3)
	   		kare=80;
		else
	   		kare=160;
	   		
	   	printf("\n\nMaaliyeti en kucuk kare platform:%d*%d\n",kare/10,kare/10);	
    	printf("En kucuk toplam maaliyet:%d\tToplam platform maaliyeti:%d\tToplam sondaj maaliyeti:%d\n",kucuk,toplamPlatform[kucukindis],toplamSondaj[kucukindis]);
    	printf("\nToplam matematiksel kar: %d\t\tToplam birim kareye gore tahmini kar: %d\n", toplamgerRezerv-kucuk,toplamtahRezerv-kucuk); // Kar hesaplan�r ve yaz�l�r.
        
		optimalCizim(x,y,k,kare); //Optimal �izim i�in fonksiyon �a��r�l�r.
        
		curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    
}
return 0;

}

