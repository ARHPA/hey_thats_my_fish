//be name khoda


//Amir Reza Hossein Pour
//Najmeh Hosseini

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

struct ICE{
	int x; // mokhtasat ICE ha
	int y;
	int status; // Ab shode || Ab nashode
	int num_fish; // Tedade mahi ha
	int UL; // ICE Up-Left
	int UR; // ICE Up-Right
	int Dl; // ICE Down-Left
	int DR; // Down-Right
	int is_full; // penguin darre ya nadare
	int num_p; // shomare penguin
	int user_p; // panguin male kodum usere
} ice[61];

struct USER{
	int score = 0;
	int number_of_active_penguin = 0;
}user[4];

struct PENGUIN{
	int p_ice; // panguin ru kodum ICE hast
	int status;// vaziat panguin / Faale ya hazf shode
	int p_user; // panguin male kodun usere
}penguin[4][4];

void must_init(bool test, const char *description) { // check kardan dorost amal kardane functions
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int check(int x, int y) { // check kardan inkero ICE hast ya na
	for (int i = 0; i < 60; i++)
		if (abs(ice[i].x - x) < 45 && abs(ice[i].y - y) < 45){
			return i;
		}
	return 60;
}

int is_near(int a, int b) { // check kardane masir- a to b
	if (ice[b].is_full == 1)
		return 0;
	int c = a - 1;
	while(ice[c].y == ice[a].y && c >= 0) {
		if(ice[c].is_full == 1)
			break;
		if (c == b)
			return 1;
		c--;
	}
	c = a + 1;
	while(ice[c].y == ice[a].y && c < 60) {
		if(ice[c].is_full == 1)
			break;
		if (c == b)
			return 1;
		c++;
	}
	c = ice[a].UL;
	while(c != 60) {
		if(ice[c].is_full == 1)
			break;
		if (c == b)
			return 1;
		c = ice[c].UL;
	}
	c = ice[a].UR;
	while(c != 60) {
		if(ice[c].is_full == 1)
			break;
		if (c == b)
			return 1;
		c = ice[c].UR;
	}
	c = ice[a].Dl;
	while(c != 60) {
		if(ice[c].is_full == 1)
			break;
		if (c == b)
			return 1;
		c = ice[c].Dl;
	}
	c = ice[a].DR;
	while(c != 60) {
		if(ice[c].is_full == 1)
			break;
		if(c == b)
			return 1;
		c = ice[c].DR;
	}	
	return 0;
}

void number_of_round_ice() {	// check kardane inke panguin hanuz mitune bazi kone ya bayad hazf beshe
	for (int i = 0; i < 60; i++) {
		if (ice[i].status != 0 && ice[i].is_full == 1) {
			int num = 6;
			if (ice[i].Dl == 60 || ice[ice[i].Dl].is_full == 1)
				num--;
			if (ice[i].DR == 60 || ice[ice[i].DR].is_full == 1)
				num--;
			if (ice[i].UL == 60 || ice[ice[i].UL].is_full == 1)
				num--;
			if (ice[i].UR == 60 || ice[ice[i].UR].is_full == 1)
				num--;
			if (i == 0 || ice[i - 1].y != ice[i].y || ice[i - 1].is_full == 1)
				num--;
			if (i == 59 || ice[i + 1].y != ice[i].y || ice[i + 1].is_full == 1)
				num--;
			if (num == 0) {
				penguin[ice[i].user_p][ice[i].num_p].status = 0;
				ice[i].status = 0;
				user[ice[i].user_p].number_of_active_penguin--;
			}
		}
	}
}

bool end(int a) { // check kardane etmame game
	int counter = a;
	for (int i = 0; i < a; i++) {
		if (user[i].number_of_active_penguin == 0) {
			counter--;
		}
	}
	if (counter == 0)
		return true; // bazi tamum shode
	else
		return false; // bazi tamum nashode
}



int getRand(int fish_groups[])
{
    int count = 3; // tedade gorohe mahi haye mored random shodan    c[1 , 2 , 3]   rc[-1 , -1 , -1]
    int c[3] = {1,2,3}; // value of fishes
    for(int i=0;i<3;i++){
        if(fish_groups[i] == 0){
            count--;
            c[i] = 0 ;
        }
    }
    int rc[3]={-1}; // [1,3]
         
    // c [1,0,3] => rc [1,3,-1] count=2
    int j = 0 ;
	for(int i=0;i<3;i++){
        if(c[i] != 0){
            rc[j]=c[i];
            j++;
        }
    }
    // rand operation : example : rc = [1,3] => 1 or 3 & count =2
    int rnd=0;
    if(count==3)
        rnd = rand() % 3 + 1;
	else if(count==2){
        rnd = rand() % 2 + 1;
        if(rnd==1)
            rnd = rc[0];
        else
            rnd = rc[1];
    }else if(count==1){
        rnd = rc[0];
    }
        
    return rnd  ; // adade tasadofi beyne {1,2,3}
}



int main() {
	// first init
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_install_mouse(), "mouse");
    //add music
    must_init(al_install_audio(), "audio");
	ALLEGRO_SAMPLE *sample = NULL;

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	
    ALLEGRO_DISPLAY* disp = al_create_display(1000,850); // andaze safe game
    al_clear_to_color(al_map_rgb(41,138,206)); // background color
    must_init(disp, "display");

    // Tarif font
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *font = al_load_font("arial.ttf",40,0);
	
	// load music
	must_init(al_init_acodec_addon(), "acodec addon");
	al_reserve_samples(1); // tedade music
	sample = al_load_sample("background_music.ogg");

	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish1 = al_load_bitmap("fish1.bmp");
    must_init(fish1, "fish1");

	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish2 = al_load_bitmap("fish2.bmp");
    must_init(fish2, "fish2");
    
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish3 = al_load_bitmap("fish3.bmp");
    must_init(fish3, "fish3");
    
    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish_1 = al_load_bitmap("fish_1.bmp");
    must_init(fish_1, "fish_1");

	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish_2 = al_load_bitmap("fish_2.bmp");
    must_init(fish_2, "fish_2");
    
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* fish_3 = al_load_bitmap("fish_3.bmp");
    must_init(fish3, "fish_3");
	
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* blue = al_load_bitmap("blue.bmp");
    ALLEGRO_BITMAP* press2 = al_load_bitmap("press2.bmp");
    ALLEGRO_BITMAP* press3 = al_load_bitmap("press3.bmp");
    ALLEGRO_BITMAP* press4 = al_load_bitmap("press4.bmp");
    ALLEGRO_BITMAP* welcome = al_load_bitmap("welcome.bmp");
    ALLEGRO_BITMAP* box_red_score = al_load_bitmap("box_red_score.bmp");
    ALLEGRO_BITMAP* box_green_score = al_load_bitmap("box_green_score.bmp");
    ALLEGRO_BITMAP* box_yellow_score = al_load_bitmap("box_yellow_score.bmp");
    ALLEGRO_BITMAP* box_purple_score = al_load_bitmap("box_purple_score.bmp");
    must_init(blue, "blue");
    must_init(press2, "press2");
    must_init(press3, "press3");
    must_init(press3, "press4");
    must_init(welcome, "welcome");
    must_init(box_red_score, "box_red_score");
    must_init(box_green_score, "box_green_score");
    must_init(box_yellow_score, "box_yellow_score");
    must_init(box_purple_score, "box_purple_score");

	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* penguin1 = al_load_bitmap("penguin1.bmp");
    ALLEGRO_BITMAP* penguin11 = al_load_bitmap("penguin11.bmp");
    must_init(penguin1, "penguin1");
    must_init(penguin11, "penguin11");
	
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* penguin2 = al_load_bitmap("penguin2.bmp");
    ALLEGRO_BITMAP* penguin22 = al_load_bitmap("penguin22.bmp");
    must_init(penguin2, "penguin2");
    must_init(penguin22, "penguin22");
	
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* penguin3 = al_load_bitmap("penguin3.bmp");
    ALLEGRO_BITMAP* penguin33 = al_load_bitmap("penguin33.bmp");
    must_init(penguin3, "penguin3");
    must_init(penguin33, "penguin33");
	
	must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* penguin4 = al_load_bitmap("penguin4.bmp");
    ALLEGRO_BITMAP* penguin44 = al_load_bitmap("penguin44.bmp");
    must_init(penguin4, "penguin4");
    must_init(penguin44, "penguin44");
	    
    must_init(al_init_primitives_addon(), "primitives");
	
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_register_event_source(queue, al_get_mouse_event_source());     
	// end first init
	
	
	// play music
	al_play_sample(sample ,1 ,0 ,1 ,ALLEGRO_PLAYMODE_LOOP , NULL);
	
    bool done = false;
    ALLEGRO_EVENT event;
    float x, y;
    int id = 60, last_id = 60;
    int ice_length = 90;
	int turn = 0; // nobat
	int part = 1; // bakhsh haye bazi
	bool starter = true;
	int cnt = 0;	
	int num_user = 0; // tedade player game
	bool end_show = false;
	bool init_penguin = false;


    #define KEY_SEEN     1
    #define KEY_RELEASED 2
    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));
	ALLEGRO_MOUSE_STATE state;
	//**************************************************************************
	srand(time(NULL));//tolid adad tasadofi
	int fish_groups[3] = {30,20,10}; // namayende tedade mahi ha
	//**************************************************************************
	//moshakhas kardan ice haye mojaver (mahdude ICE ha baraye game)
	for (int i = 0; i < 60; i++) {
		// mojaver haye bala
		if (i <= 7) {
			ice[i].UL = 60;
			ice[i].UR = 60;
		}
		else {
			if (i % 15 == 0) //goshe haye chap
				ice[i].UL = 60;
			else
				ice[i].UL = i - 8;
			if (i % 15 == 7) //goshe haye rast
				ice[i].UR = 60;
			else
				ice[i].UR = i - 7;
		}
		// mojaver haye paeen
		if(i >= 53) {
			ice[i].Dl = 60;
			ice[i].DR = 60;
		}
		else {
			if (i % 15 == 0) //goshe haye chap
				ice[i].Dl = 60;
			else
				ice[i].Dl = i + 7;
			if(i % 15 == 7) //goshe haye rast
				ice[i].DR = 60;
			else
				ice[i].DR = i + 8;
		}
	//************** moshakhas kardane tedede mahi ha / start : **************
    // the final loop for fishs
        int r = getRand(fish_groups);
        fish_groups[r-1]--;
//        printf("%d\t", r );
        if (r == 3)
        	ice[i].num_fish = 3 ;
        else if(r == 2)
        	ice[i].num_fish = 2 ;
		else if(r == 1)
        	ice[i].num_fish = 1 ;
	//************** moshakhas kardane tedede mahi ha / End ******************
		
		ice[i].status = 1;
		ice[i].is_full = 0;
	}
	//-------------------------
	//makan ice ha
    int h = 15, d, counter = 0, X = 15;
    for (int j = 0; j < 8; j++) {
		if (j % 2 == 0)
			for (int i = 0; i < 8; i++) {
    			ice[counter].x = X + 45;
    			ice[counter].y = h + 45;
				X += 100;
				counter++;
			}
		else
			for (int i = 0; i < 7; i++) {
				ice[counter].x = X + 45 + 45;
    			ice[counter].y = h + 45;
				X += 100;
				counter++;
			}
		h += 89;
		X = 15;
	}
	
	//********main while******************************************************************************************
    al_start_timer(timer);
    while(1) {
        al_wait_for_event(queue, &event);
		switch(event.type)
		{
		    case ALLEGRO_EVENT_TIMER:
		    	if(!num_user && key[ALLEGRO_KEY_2])
		            num_user = 2;
		    	if(!num_user && key[ALLEGRO_KEY_3])
		            num_user = 3;
		    	if(!num_user && key[ALLEGRO_KEY_4])
		            num_user = 4;
		        if(key[ALLEGRO_KEY_ESCAPE])
		            done = true;
		        for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
		            key[i] &= KEY_SEEN;
		        break;
		
		    case ALLEGRO_EVENT_MOUSE_AXES:
		    	al_get_mouse_state(&state);
		    	if (al_mouse_button_down(&state, 1)) {
		    	    x = state.x;
		    	    y = state.y;
		    	    id = check(x, y);
		        }
		        break;
		    case ALLEGRO_EVENT_KEY_DOWN:
		        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
		        break;
		    case ALLEGRO_EVENT_KEY_UP:
		        key[event.keyboard.keycode] &= KEY_RELEASED;
		        break;			
		    case ALLEGRO_EVENT_DISPLAY_CLOSE:
		        done = true;
		        break;
		}
		if(done)
            break;
	    if (!num_user) // if num user moshkhas nasode bud hanuz :
		{
	    	al_draw_bitmap(welcome, 0, 0, 0); // namaesh safhe welcome
	    	al_flip_display();
		}   
		else if (num_user && !init_penguin) { // agar num user moshakhas shode vali panguin rasm nashode
			//************** namayesh axe "You press num i" **************
			if(num_user == 2){
				al_draw_bitmap(press2, 0, 0, 0);
				al_flip_display();
				al_rest(1);
			}
			else if(num_user == 3){
				al_draw_bitmap(press3, 0, 0, 0);
				al_flip_display();
				al_rest(1);
			}
			else if(num_user == 4){
				al_draw_bitmap(press4, 0, 0, 0);
				al_flip_display();
				al_rest(1);
			}
	    	//************** init penguin status **************
			for (int i = 0; i < num_user; i++) {
				for (int j = 0; j < 6 - num_user; j++) {
					penguin[i][j].status = 0;
					user[i].number_of_active_penguin++;
				}
			}	
			init_penguin = true;		
		}
        else if (starter && !end_show) {
			if (id != 60 && ice[id].is_full == 0) {
				ice[id].is_full = 1;
				ice[id].num_p = cnt;
				ice[id].user_p = turn;
				penguin[turn][cnt].p_ice = id;
				penguin[turn][cnt].status = 1;
				user[turn].score += ice[id].num_fish;
				turn++;
				if (turn >= num_user) {
					turn = 0;
					cnt++;
				}
				if (cnt >= 6 - num_user)
					starter = 0;
				number_of_round_ice(); // agar hame panguin haye player ghable shoro bazi hazf shodan ....
			}		
			// namayesh nobat
			al_clear_to_color(al_map_rgb(41,138,206));
 			al_draw_bitmap(blue, 0, 0, 0);
			if (turn == 0)
				al_draw_textf(font,al_map_rgb(255,255,255),30,775,0,"Green Player Select Your Penguin Location");
			else if (turn == 1)
				al_draw_textf(font,al_map_rgb(255,255,255),50,775,0,"Red Player Select Your Penguin Location");
			else if (turn == 2)
				al_draw_textf(font,al_map_rgb(255,255,255),20,775,0,"Yellow Player Select Your Penguin Location");
			else
				al_draw_textf(font,al_map_rgb(255,255,255),20,775,0,"Purple Player Select Your Penguin Location");
			//************** rasm ice ha **************
            int counter = 0;
            for (int i = 0; i < 8; i++) {
            	if (i % 2 == 0) {
            		for (int j = 0; j < 8; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1)
            				al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            			else if (ice[counter].num_fish == 2)
							al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						else
							al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						counter++;
					}
				}
            	else {
            		for (int j = 0; j < 7; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1)
            				al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            			else if (ice[counter].num_fish == 2)
							al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						else
							al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						counter++;
					}
				}
			}
			//************** rasm penguin ha **************
			for (int i = 0; i < num_user; i++) {
				for (int j = 0; j < 6 - num_user; j++) {
					if (penguin[i][j].status == 1) {
						if (i == 0) {
							al_draw_bitmap(penguin1, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						}
						if (i == 1) {
							al_draw_bitmap(penguin2, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						}
						if (i == 2)
							al_draw_bitmap(penguin3, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 3)
							al_draw_bitmap(penguin4, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
					}
				}				
			}
 			// namayesh score box
 			if(num_user == 2){ // agar bazi 2 nafare bud
 				al_draw_bitmap(box_green_score,830,185, 0);
 				al_draw_bitmap(box_red_score,830,420, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,275,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,275,0,"%d",user[0].score);
 				if (user[1].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,510,0,"%d",user[1].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,510,0,"%d",user[1].score);
			}
			else if(num_user == 3){ //agar bazi 3 nafare bud
				al_draw_bitmap(box_green_score,830,90, 0);
 				al_draw_bitmap(box_red_score,830,300, 0);
 				al_draw_bitmap(box_yellow_score,830,510, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,180,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,180,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,390,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,390,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,600,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,600,0,"%d",user[2].score);
			}
			else if(num_user == 4) { //agar bazi 4 nafare bud
				al_draw_bitmap(box_green_score,830,50, 0);
 				al_draw_bitmap(box_red_score,830,220, 0);
 				al_draw_bitmap(box_yellow_score,830,390, 0);
 				al_draw_bitmap(box_purple_score,830,560, 0);
 				if (user[0].score < 10)
	 				al_draw_textf(font,al_map_rgb(255,255,255),880,140,0,"%d",user[0].score);
 				else
	 				al_draw_textf(font,al_map_rgb(255,255,255),870,140,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				if (user[3].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
			}
			al_flip_display();
		}
		else if (part == 1 && !end_show) 
		{
			if (ice[id].status == 1 && ice[id].is_full == 1 && ice[id].user_p == turn) {
				last_id = id;
				part = 2;
			}
			al_clear_to_color(al_map_rgb(41,138,206));
 			al_draw_bitmap(blue, 0, 0, 0);
			if (turn == 0)
				al_draw_textf(font,al_map_rgb(255,255,255),30,775,0,"Green Player Select One of Your Penguin");
			else if (turn == 1)
				al_draw_textf(font,al_map_rgb(255,255,255),50,775,0,"Red Player Select One of Your Penguin");
			else if (turn == 2)
				al_draw_textf(font,al_map_rgb(255,255,255),20,775,0,"Yellow Player Select One of Your Penguin");
			else
				al_draw_textf(font,al_map_rgb(255,255,255),20,775,0,"Purple Player Select One of Your Penguin");
			//************** rasm ice ha **************
            int counter = 0;
            for (int i = 0; i < 8; i++) {
            	if (i % 2 == 0) {
            		for (int j = 0; j < 8; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1)
            				al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            			else if (ice[counter].num_fish == 2)
							al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						else
							al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						counter++;
					}
				}
            	else {
            		for (int j = 0; j < 7; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1)
            				al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            			else if (ice[counter].num_fish == 2)
							al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						else
							al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						counter++;
					}
				}
			}
			//************** rasm penguin ha **************
			for (int i = 0; i < num_user; i++) {
				for (int j = 0; j < 4; j++) {
					if (penguin[i][j].status == 1) {
						if (i == 0)
							al_draw_bitmap(penguin1, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 1) 
							al_draw_bitmap(penguin2, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 2)
							al_draw_bitmap(penguin3, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 3)
							al_draw_bitmap(penguin4, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
					}
				}	
			}
 			// namayesh score box
 			if(num_user == 2){ //agar bazi 2 nafare bud
 				al_draw_bitmap(box_green_score,830,185, 0);
 				al_draw_bitmap(box_red_score,830,420, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,275,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,275,0,"%d",user[0].score);
 				if (user[1].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,510,0,"%d",user[1].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,510,0,"%d",user[1].score);
			}
			else if(num_user == 3){ ////agar bazi 3 nafare bud
				al_draw_bitmap(box_green_score,830,90, 0);
 				al_draw_bitmap(box_red_score,830,300, 0);
 				al_draw_bitmap(box_yellow_score,830,510, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,180,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,180,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,390,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,390,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,600,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,600,0,"%d",user[2].score);
			}
			else if(num_user == 4) { ////agar bazi 4 nafare bud
				al_draw_bitmap(box_green_score,830,50, 0);
 				al_draw_bitmap(box_red_score,830,220, 0);
 				al_draw_bitmap(box_yellow_score,830,390, 0);
 				al_draw_bitmap(box_purple_score,830,560, 0);
 				if (user[0].score < 10)
	 				al_draw_textf(font,al_map_rgb(255,255,255),880,140,0,"%d",user[0].score);
 				else
	 				al_draw_textf(font,al_map_rgb(255,255,255),870,140,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				if (user[3].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
			}
			al_flip_display();
	    }
	    else if (part == 2 && !end_show) 
		{
			if(ice[id].status == 1 && ice[id].is_full == 0 && is_near(last_id, id) == 1) {
				ice[last_id].status = 0;
				penguin[ice[last_id].user_p][ice[last_id].num_p].p_ice = id;
				ice[id].is_full = 1;
				ice[id].num_p = ice[last_id].num_p;
				ice[id].user_p = ice[last_id].user_p;
				user[ice[id].user_p].score += ice[id].num_fish;
				number_of_round_ice();
				part = 1;
				if (end(num_user) == 1) {
					end_show = true;
					continue;
				}
				turn++;
				if (turn >= num_user)
					turn = 0;
				while(user[turn].number_of_active_penguin == 0) {
					turn++;
					if (turn >= num_user)
						turn = 0;
				}
			}
			else if (ice[id].status == 1 && ice[id].is_full == 1 && ice[id].user_p == turn)
				last_id = id;
			// namayesh nobat
			al_clear_to_color(al_map_rgb(41,138,206));
 			al_draw_bitmap(blue, 0, 0, 0);
			if (turn == 0)
				al_draw_textf(font,al_map_rgb(255,255,255),110,775,0,"Green Player Select Next Location");
			else if (turn == 1)
				al_draw_textf(font,al_map_rgb(255,255,255),130,775,0,"Red Player Select Next Location");
			else if (turn == 2)
				al_draw_textf(font,al_map_rgb(255,255,255),100,775,0,"Yellow Player Select Next Location");
			else
				al_draw_textf(font,al_map_rgb(255,255,255),100,775,0,"Purple Player Select Next Location");
			//************** rasm ice ha **************
            int counter = 0;
            for (int i = 0; i < 8; i++) {
            	if (i % 2 == 0) {
            		for (int j = 0; j < 8; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1) {
							if (is_near(last_id, counter) == 1)
            					al_draw_bitmap(fish_1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            				else
            					al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);	
            			}
            			else if (ice[counter].num_fish == 2) {
            				if (is_near(last_id, counter) == 1)
								al_draw_bitmap(fish_2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
							else
								al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						}
						else {
							if (is_near(last_id, counter) == 1)
								al_draw_bitmap(fish_3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
							else
								al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						}
						counter++;
					}
				}
            	else {
            		for (int j = 0; j < 7; j++) {
            			if (ice[counter].status == 0) {
            				counter++;
            				continue;
						}
            			if (ice[counter].num_fish == 1) {
							if (is_near(last_id, counter) == 1)
            					al_draw_bitmap(fish_1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
            				else
            					al_draw_bitmap(fish1, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						}
            			else if (ice[counter].num_fish == 2) {
            				if (is_near(last_id, counter) == 1)
								al_draw_bitmap(fish_2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
							else
								al_draw_bitmap(fish2, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						}
						else {
							if (is_near(last_id, counter) == 1)
								al_draw_bitmap(fish_3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
							else
								al_draw_bitmap(fish3, ice[counter].x - (ice_length / 2), ice[counter].y - (ice_length / 2), 0);
						}
						counter++;
					}
				}
			}
			//************** rasm penguin ha **************
			for (int i = 0; i < num_user; i++) {
				for (int j = 0; j < 4; j++) {
					if (penguin[i][j].status == 1) {
						if (i == 0)
							al_draw_bitmap(penguin1, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 1)
							al_draw_bitmap(penguin2, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 2)
							al_draw_bitmap(penguin3, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
						if (i == 3)
							al_draw_bitmap(penguin4, ice[penguin[i][j].p_ice].x - 45, ice[penguin[i][j].p_ice].y - 40, 0);
					}
				}
			}
 			// namayesh score box
 			
 			if(num_user == 2){ //agar bazi 2 nafare bud
 				al_draw_bitmap(box_green_score,830,185, 0);
 				al_draw_bitmap(box_red_score,830,420, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,275,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,275,0,"%d",user[0].score);
 				if (user[1].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,510,0,"%d",user[1].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,510,0,"%d",user[1].score);
			}
			else if(num_user == 3){ //agar bazi 3 nafare bud
				al_draw_bitmap(box_green_score,830,90, 0);
 				al_draw_bitmap(box_red_score,830,300, 0);
 				al_draw_bitmap(box_yellow_score,830,510, 0);
 				if (user[0].score < 10)
 					al_draw_textf(font,al_map_rgb(255,255,255),880,180,0,"%d",user[0].score);
 				else
 					al_draw_textf(font,al_map_rgb(255,255,255),870,180,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,390,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,390,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,600,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),870,600,0,"%d",user[2].score);
			}
			else if(num_user == 4) { //agar bazi 4 nafare bud
				al_draw_bitmap(box_green_score,830,50, 0);
 				al_draw_bitmap(box_red_score,830,220, 0);
 				al_draw_bitmap(box_yellow_score,830,390, 0);
 				al_draw_bitmap(box_purple_score,830,560, 0);
 				if (user[0].score < 10)
	 				al_draw_textf(font,al_map_rgb(255,255,255),880,140,0,"%d",user[0].score);
 				else
	 				al_draw_textf(font,al_map_rgb(255,255,255),870,140,0,"%d",user[0].score);
 				if (user[1].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,310,0,"%d",user[1].score);
				if (user[2].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,480,0,"%d",user[2].score);
				if (user[3].score < 10)
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
				else
					al_draw_textf(font,al_map_rgb(255,255,255),880,650,0,"%d",user[3].score);
			}
			number_of_round_ice();	// agar hame panguin haye player ghable shoro bazi hazf shodan ....
            al_flip_display();
		}
 		else 
		{
			//************** Moshakhas kardan barande **************
 			int max = 0, user_name = 0;
 			for (int i = 0; i < num_user; i++)
 				if (user[i].score > max) {
 					user_name = i;
 					max = user[i].score;
				 }
			// namayeshe Barande & score barande :
			if (user_name == 0) {
				al_clear_to_color(al_map_rgb(41,138,206));
				al_draw_bitmap(penguin11, 0, 0, 0);
				al_draw_textf(font,al_map_rgb(255,255,255),310,280,0,"%d",max);
			}
			else if (user_name == 1) {
				al_clear_to_color(al_map_rgb(41,138,206));
				al_draw_bitmap(penguin22, 0, 0, 0);
				al_draw_textf(font,al_map_rgb(255,255,255),310,280,0,"%d",max);
			}
			else if (user_name == 2) {
				al_clear_to_color(al_map_rgb(41,138,206));
				al_draw_bitmap(penguin33, 0, 0, 0);
				al_draw_textf(font,al_map_rgb(255,255,255),310,280,0,"%d",max);
			}
			else {
				al_clear_to_color(al_map_rgb(41,138,206));
				al_draw_bitmap(penguin44, 0, 0, 0);
				al_draw_textf(font,al_map_rgb(255,255,255),310,280,0,"%d",max);
			}
 			al_flip_display();

		}
    }
    //************** destroy **************
    al_destroy_bitmap(penguin1);
    al_destroy_bitmap(penguin11);
    al_destroy_bitmap(penguin2);
    al_destroy_bitmap(penguin22);
    al_destroy_bitmap(penguin3);
    al_destroy_bitmap(penguin33);
    al_destroy_bitmap(penguin4);
    al_destroy_bitmap(penguin44);
    al_destroy_bitmap(blue);    
    al_destroy_bitmap(welcome);    
    al_destroy_bitmap(press2);    
    al_destroy_bitmap(press3);    
    al_destroy_bitmap(press4);    
    al_destroy_bitmap(box_red_score);    
    al_destroy_bitmap(box_green_score);    
    al_destroy_bitmap(box_yellow_score);    
    al_destroy_bitmap(box_purple_score);    
    al_destroy_bitmap(fish1);
	al_destroy_bitmap(fish2);
	al_destroy_bitmap(fish3);
	al_destroy_bitmap(fish_1);
	al_destroy_bitmap(fish_2);
	al_destroy_bitmap(fish_3);
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_sample(sample);
    al_destroy_event_queue(queue);    
    return 0;
}
