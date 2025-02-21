#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <curl/curl.h>
#include <graphics.h>

// 220201109 ESENGÜL TURAN 220201102 CANAN KORKUT PROLAB PROJE1

int kontrol = 0; //Kodun çalýþma kontrolü için kullanýlan bir global deðiþken

//Çokgenin alanýný Gauss alan formülü ie hesaplama (shoelace formülü olarak da bilinir)
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

//Noktanýn çokgenin içinde olup olmadýðýný kontrol eden fonksiyon.
int icindeMi(int x[], int y[], int n, int noktaX, int noktaY) {
    int i, j, d = 0;

    for (int i = 0; i < n; i++) { //Noktanýn çokgenin içinde olup olmadýðýný kontrol eden çokgen kenarlarýný dolaþan döngü

        int j = i - 1;
        if (i == 0) {
            j = n - 1;
        }

        if ((y[i] > noktaY) != (y[j] > noktaY) && (noktaX < (x[j] - x[i]) * (noktaY - y[i]) / (y[j] - y[i]) + x[i])) {
            d = !d;  // Nokta, çizgiyi kesiyorsa veya çizginin sýnýr noktasýndan geçiyorsa, içinde sayýlýr.
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
        if (x[i] < minX) {                 //Çokgenin sýnýrlarý hesaplanýr.
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

    for (int i = minX; i <= maxX; i += kare) {      //X ve Y koordinatlarý arasýnda dolaþýr.
        for (int j = minY; j <= maxY; j += kare) {
            int icindeNoktaVar = 0;

            for (int xKonum = 0; xKonum < kare; xKonum++) {   //Her birim kareyi inceler.
                for (int yKonum = 0; yKonum < kare; yKonum++) {
                    int xGonder = i + xKonum;
                    int yGonder = j + yKonum;

                    //Eðer birim karenin içinde çokgenin bir noktasý varsa, icindeNoktaVar'ý 1 yapar ve döngüyü kýrar.
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
                kareSayisi++;                      //Eðer birim kare çokgenin içindeyse, birim kare sayýsýný arttýrýr.
                if (kontrol == 0) {
                    bar(i, j, i + kare, j + kare); //Eðer birim kare çokgenin içindeyse, boyayý ekler.
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
    initgraph(&gd, &gm, "");  //Grafik modunu baþlatýr.
    setbkcolor(WHITE);
    cleardevice();			 //Pencereyi temizler.

    int k = s;

    setcolor(8);

    // Izgara çizer.
    for (int i = 1; i < 10000; i++) {
        line(i * 10, 0, i * 10, 10000);
    }

    for (int j = 1; j < 10000; j++) {
        line(0, j * 10, 10000, j * 10);
    }

    
    int baslangicNoktasiX[k / 2] = {0};  //Her noktanýn baþlangýç noktasý olup olmadýðýný izlemek için dizi.
    int baslangicNoktasiY[k / 2] = {0};

    for (int i = 0; i < k / 2; i++) {
        if (baslangicNoktasiX[i] == 1 && baslangicNoktasiY[i] == 1) {
            setfillstyle(SOLID_FILL, RED);
            fillellipse(x[i], y[i], 4, 4); //Eðer baþlangýç noktasý ise, o noktayý kýrmýzý bir daire çizer.
            continue;  // Bu zaten bir baþlangýç noktasý ise atlar.
        }
        for (int j = i + 1; j < k / 2; j++) {
            if (x[i] == x[j] && y[i] == y[j]) {
                baslangicNoktasiX[j] = 1;
                baslangicNoktasiY[j] = 1;

                int n = j - i;   //Çokgenin köþe sayýsý
                int px[n], py[n], p[2 * n];  //Çokgenin x ve y koordinatlarýný tutacak diziler px py ve tüm koordinatlarýný tutan p dizisi.
                for (int a = i; a < j; a++) {
                    px[a - i] = x[a];
                    py[a - i] = y[a];
                }
                for (int a = i; a <= j; a++) {
                    p[2 * (a - i)] = x[a];     //Çokgenin x ve y koordinatlarýný tek bir dizi olan p dizisine kopyalar.
                    p[2 * (a - i) + 1] = y[a];
                }

                setfillstyle(SOLID_FILL, LIGHTGRAY);
                kontrol = 0;
                boyaCokgen(px, py, n, kare);  //Optimal cizim icin gönderilen kare deðiþkinene göre platformlarý boyar.
                
				setfillstyle(SOLID_FILL, DARKGRAY);
                fillpoly(n, p);  //Optimal cizim icin rezerv alanlarýný boyar.
                
				kontrol = 2;
                boyaCokgen(px, py, n, kare); //Optimal cizim icin platformlarý çizer.

            }
        }
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(x[i], y[i], 4, 4); //Köþe noktalarýna siyah bir daire çizer.
        setbkcolor(BLACK);
        line(x[i], y[i], x[i + 1], y[i + 1]); //Çokgeni çizer.
    }

    delay(50000000);
    closegraph();
}

// HTTP üzerinden veri çekme iþlemi için kullanýlan fonksiyon
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
    	// HTTP üzerinden veri çekme iþlemi
        curl_easy_setopt(curl, CURLOPT_URL, "http://bilgisayar.kocaeli.edu.tr/prolab1/prolab1.txt");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, veriCekme);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, veri);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() basarýsýz: %s\n", curl_easy_strerror(res));
        } 
	else {
            printf("%s\n\n", veri);
            int dizi[3000];
            int k = 0, m = 0;
            int satir = 0;
            printf("Lutfen kacinci satiri istediginizi giriniz:");
            scanf("%d", &satir);
            
            char *karakter;		//Satýr baþlangýçlarý için pointer.
            
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

            const char *ayrac = "B(,)F";   //Ayrac'a göre karakterleri ayýrýr.
            karakter = strtok(karakter + 2, ayrac);
            
            while (karakter != NULL && *karakter != '\n' && *karakter != '\0') {
                dizi[k] = atoi(karakter);     //Ýnteger deðere çevirir.
                k++;
                karakter = strtok(NULL, ayrac);
            }
            

            int gd = DETECT, gm;
            initgraph(&gd, &gm, ""); //Grafik modunu baþlatýr.
            setbkcolor(WHITE);
            cleardevice();       //Pencereyi temizler.

            for (int i = 0; i < k; i++) {
                dizi[i] = dizi[i] * 10;
            }

            int x[k / 2], y[k / 2];   //Çokgenlerin x ve y koordinatlarý için diziler.
            for (int i = 0; i < k / 2; i++) {
                x[i] = dizi[i * 2];
                y[i] = dizi[i * 2 + 1];
            }

            setcolor(8);

            // Izgara çizer.
            for (int i = 1; i < 10000; i++) {
                line(i * 10, 0, i * 10, 10000);
            }

            for (int j = 1; j < 10000; j++) {
                line(0, j * 10, 10000, j * 10);
            }

            int baslangicNoktasiX[k / 2] = {0};  //Her noktanýn baþlangýç noktasý olup olmadýðýný izlemek için bir dizi.
            int baslangicNoktasiY[k / 2] = {0};

            int cokgenSayisi = 0;
            int cokgenAlanlari[50];
            int alan2[50];
            int alan3[50][50];  //Çokgenlerin optimal çizimleri için platformlara göre alanlarýný tutan 2 boyutlu dizi.

            for (int i = 0; i < k / 2; i++) {

                if (baslangicNoktasiX[i] == 1 && baslangicNoktasiY[i] == 1) {
                    setfillstyle(SOLID_FILL, RED);
                    fillellipse(x[i], y[i], 4, 4);//Eðer baþlangýç noktasý ise, o noktayý kýrmýzý bir daire çizer.
            		continue;  // Bu zaten bir baþlangýç noktasý ise atlar.
                }
                for (int j = i + 1; j < k / 2; j++) {
                    if (x[i] == x[j] && y[i] == y[j]) {
                        baslangicNoktasiX[j] = 1;
                        baslangicNoktasiY[j] = 1;

                        printf("\nBaslangic Noktasi olan %d. parantezdeki koordinatlar: x=%d, y=%d \n", i + 1, x[i] / 10, y[i] / 10); 

                        int n = j - i;  //Çokgenin köþe sayýsý
                        int px[n], py[n]; //Çokgenin x ve y koordinatlarýný tutacak diziler px py. 
                        for (int a = i; a < j; a++) {
                            px[a - i] = x[a];
                            py[a - i] = y[a];
                        }

                        setfillstyle(SOLID_FILL, LIGHTGRAY);
                        kontrol = 0;
                        alan2[cokgenSayisi] = boyaCokgen(px, py, n, 10); //boyaCokgen 1*1'e göre boyama yapar ve alaný birim kare cinsinden döndürür,alan2 dizisi alanlarý tutar.
                        

                        cokgenSayisi++;
                        int alan = cokgenAlani(n, px, py);  //Shoelace formülü ile alan hesaplar.(Matematiksel alan)
                        cokgenAlanlari[cokgenSayisi - 1] = alan; //Matematiksel alanlarý bir dizide tutar.

                        int b = 0;
                        for (int i = 10; i <= 160; i *= 2) {

                            kontrol = 1;
                            alan3[cokgenSayisi - 1][b] = boyaCokgen(px, py, n, i); //Çokgenlerin optimal çizimleri için (1*1,2*2,4*4,8*8,16*16)'a göre alanlarýný hesaplar.
                            b++;
                        }
                    }
                }
                setfillstyle(SOLID_FILL, BLACK);
                fillellipse(x[i], y[i], 4, 4); //Köþe noktalarýna siyah bir daire çizer.
                setbkcolor(BLACK);
                line(x[i], y[i], x[i + 1], y[i + 1]); //Çokgeni çizer.
            }

            printf("\n\nCokgen sayisi: %d\n", cokgenSayisi);
            for (int i = 0; i < cokgenSayisi; i++) {
                printf("%d. Cokgenin matematiksel alani: %d\t\tBirim alani: %d\n", i + 1, cokgenAlanlari[i], alan2[i]); //Çokgenlerin alanlarýný yazdýrýr.
            }

            int tahminiRezerv[100] = {0}, gercekRezerv[100] = {0};
            int toplamtahRezerv = 0, toplamgerRezerv = 0, toplam = 0;

            for (int i = 0; i < cokgenSayisi; i++) {
                toplam += alan2[i];
                tahminiRezerv[i] = alan2[i] * 10;
                gercekRezerv[i] = cokgenAlanlari[i] * 10;  //Çokgenlerin rezervleri hesaplanýr.
            }

            printf("\n");

           

	    
	    for (int i = 0; i < cokgenSayisi; i++) {
	        printf("%d. Cokgenin matematiksel rezervi: %d\t\tBirim tahmini rezerv: %d\n", i+1, gercekRezerv[i],tahminiRezerv[i]); // Çokgenlerin rezervlerini yazdýrýr.
	    }
	    
	    for (int i = 0; i < cokgenSayisi; i++) {
	    	toplamtahRezerv+=tahminiRezerv[i];
	    	toplamgerRezerv+=gercekRezerv[i];  //Toplam rezerv hesaplanýr.
	    }
	
		printf("\n\t\t\t\t\t\tToplam birim kare(1*1):%d",toplam);
	    printf("\nToplam matematiksel rezervi: %d\t\tToplam birim kareye gore tahmini rezerv: %d\n", toplamgerRezerv,toplamtahRezerv); // Çokgenlerin toplam rezervini yazdýrýr.
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
				toplamBirimkare[j]+=alan3[i][j]; //Çokgenlerin platformlara (1*1,2*2,4*4,8*8,16*16) göre alanlarýný toplayýp diziye aktarýr.

			} 
	    }
        
        printf("\n\nToplam platform sayisi(1*1,2*2,4*4,8*8,16*16)'a gore sirasiyla:\n");
	    for(int j=0;j<5;j++){    	
				
			printf("%d\t",toplamBirimkare[j]); //Toplam platform sayýlarý karesel alanlara göre (1*1,2*2,4*4,8*8,16*16)
		} 
	    
	   
	   int kare=1;
       
	   for(int j=0;j<5;j++){    	
			toplamPlatform[j]+=toplamBirimkare[j]*platform;  //Toplam platform maaliyeti platformlara göre hesaplanýr.(1*1,2*2,4*4,8*8,16*16)
			toplamSondajsayisi[j]=toplamBirimkare[j]*kare;   //Toplam sondaj sayýlarý platformlara göre hesaplanýr.(1*1,2*2,4*4,8*8,16*16)
			toplamSondaj[j]+=toplamBirimkare[j]*kare*sondaj; //Toplam sondaj maaliyeti platformlara göre hesaplanýr.(1*1,2*2,4*4,8*8,16*16)
			maaliyet[j]=toplamPlatform[j]+toplamSondaj[j];   //Toplam maaliyet platformlara göre hesaplanýr.(1*1,2*2,4*4,8*8,16*16)
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
				kucuk=maaliyet[j];  //En küçük maaliyet bulunur.
				kucukindis=j;
			}
	   }
	   

		if(kucukindis==0)
	   		kare=10;
    	else if(kucukindis==1)
	   		kare=20;
	   	else if(kucukindis==2)    //En küçük maaliyete göre optimal çizim için platform boyutlarý(karesel alan) belirlenir.
	   		kare=40;
	   	else if(kucukindis==3)
	   		kare=80;
		else
	   		kare=160;
	   		
	   	printf("\n\nMaaliyeti en kucuk kare platform:%d*%d\n",kare/10,kare/10);	
    	printf("En kucuk toplam maaliyet:%d\tToplam platform maaliyeti:%d\tToplam sondaj maaliyeti:%d\n",kucuk,toplamPlatform[kucukindis],toplamSondaj[kucukindis]);
    	printf("\nToplam matematiksel kar: %d\t\tToplam birim kareye gore tahmini kar: %d\n", toplamgerRezerv-kucuk,toplamtahRezerv-kucuk); // Kar hesaplanýr ve yazýlýr.
        
		optimalCizim(x,y,k,kare); //Optimal çizim için fonksiyon çaðýrýlýr.
        
		curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    
}
return 0;

}

