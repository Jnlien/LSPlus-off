enum JOBS_ENUM
{
	jobID,
	jobType,
	Float:jobX,
	Float:jobY,
	Float:jobZ,
	jobInt,
	jobVW,
	jobPickUp,
	jobOn
}
new JobsInfo[MAX_JOBS][JOBS_ENUM];

#define MAX_JOBS_TYPES 11
new jobs_names[MAX_JOBS_TYPES][32] = {
    "Aucun", 
    "Routier", 
    "Mécanicien", 
    "Chauffeur de taxi", 
    "Dealer de drogue", 
    "Vendeur d'armes", 
    "Livreur de pizzas",
    "Voleur de voitures",
    "Bûcheron",
    "Éboueur",
    "Mineur"
};