#pragma once





// ***** DO NOT FORGET UR SYNTAX "@[command]>[target]{option1/option2} *****



//TO DO
/*

MUST DOs:
	UPDATE OPTION PARSER TO USE THE COMMANDREGISTRY MAP
		- this will make it easier to add new commands and options
		- also will make it easier to validate options and targets



POLISH:

	Make the UI more user friendly
	
	+ A LOT MORE
		




PROTECTION:
	Make the .amle files encrypted.


	Maybe make the .h/ .cpp files encrypted too
		Encrypting the source code would be good.





OPTIMIZATION:
	Make Optimization Profiles
		Method:
			@config>optimize{profile = [profile name]}

			3 profiles:
				P-1. speed
					2 layers
						1(hidden): make bypasser to funciton checks
						2(user accesible): unknown yet.



				P-2. memory
					std::vector<TrainingInput> trainingInputs; does not need to be stored
						Reduce runtime memory usage by limiting or disabling
						retention of session-only TrainingInput entries.

					@config>optimize{profile=memory/absolute=true}
						absolute:
							When enabled:
								- Aggressively minimize RAM usage during training.
								- TrainingInputs are capped/limited.
								- AIMemory is not populated during training.
								- NGram training data may periodically be serialized to disk.
								- Clear in-memory maps after spilling.
								- Restore/load disk-backed data when required for generation.
								- Consider disabling higher NGram sizes (6/7) under absolute mode.
								- Do NOT destroy NgramModel objects; only release their stored data.
								- Decide how disk chunks should be merged/indexed later.



					@config>optimize{profile=memory/training-inputs-size=[<20]} if they really want to have that.
					
					AIMemory memory; is also not being filled.



				P-3. balanced(the normal one)
	

	*CAVEATS + MUST DO* 
		give the user clear feedback on what the optimization profile does

		for memory optimization profile:
			make sure to tell the user that the training inputs will not be stored in memory 
			and that the memory will not be filled with data.

		could have a little start or text to notify the user that the optimization profile is being used
			maybe a '*' symbol, also colored.
				green = memory optimization
				blue = speed optimization
				yellow = balanced optimization





GENERAL:


	!!! MUST HAVE !!! 
		Add @check(can work with options)
			Syntax: @check{ [option] } would only display the "check" for that option
				be id {model=ngram} or whatever else
			- Show subsystem existence
			- Show memory address
			- Show empty/non-empty state
			- Show subsystem-specific statistics
			- Memory statistics
			- Vocabulary statistics
			- Training-input statistics
			- Legacy transition/probability statistics
			- NGram statistics per size
			- Transition occurrence/frequency statistics
			- Debugger flag states



	Add more debugflags
		Trace:
			things like:
			"Entered [funciton name]"
			and whatnot.
			

		PARSER:
			so that when doing @[command]>[target]{ [options] }
			it prints
				[COMMAND] = [command]
				[TARGET] = [target]
				[OPTIONS] = [options]


		UI:
			Add messages for user when things are done/succeded
				Examples:
					"Training done"
					"Save Complete"/ "Loading Complete" already exist.





TOOLS:
	Add tools for AMLE:
		file tools
		network tools?!
		web searcher
		chat room???





WARNING + ERRORS:
	Add a warning bypasser? - wont print warnings
	Maybe an error bnypasser too? - wont throw
		tho... this could be... hella wierd

	Something like:
		@config>war-err { warning = false}
			... that... is quite the hassle when errors are involved
			for some it may be possible... instead of throw just bypass it and use default values??


	Separate recoverable errors from fatal errors.
	*DONE*
	{
		Recoverable:
		- Invalid option values
		- Invalid temperatures
		- Invalid booleans
		- Invalid integers
		-> Show warning
		-> Use default value
		-> Continue execution
	}


	Fatal:
		- File loading
		- Save failures
		- Corrupted JSON
		- Missing models
		- Internal logic errors
		-> Throw exception
		-> Abort current command


	Possible future setting:
		@config>runtime{
			invalid-options = use-default
		}


	*DONE*
	{
		OptionParser should eventually:
			missing option -> default
			invalid option -> warning + default (configurable)
			valid option -> parsed value
	}





PARSER:
	Make syntax flexible
		allow shit like:
			@generte>text
			{ temp = 1.5/
			  model = NGram/
			  model-size = 4
			}





FOR FUTURE:
	
	GENERATION:
		Add more parameters for generation
			Mutations:
				MutationChance = Chance of the porbs to change slightly
				MutationStrength = How much the probs can change


			Lookahead:
				How many tokens to lookahead when generating the next token
				LookAheadDepth = how far to lookahead across all bracnhes
				Influence = how much can lookahead modfiy the tokens probability

			
			Personality (for MUCH later):
				*Very slightly* changes the probabilities of the tokens
				2 Profiles (for now):
					Certainty = boosts higher probs
					Novelty = boosts lower probs
				

			THESE CAN INTERACT WITH EACH OTHER


			Pipeline Ideea:(can obviously be changed)
				1.Temperature
				2.Lookahead
				3.Mutations
				4.Personality

				*MAYBE even make the order configurable in the future*
			




	TOKENS:
		Make a whole class Token
			stores the word
			stores the id
		
		Vocabulary now has an even more important role
		Make it so the tokens ID's are awlays the same across all models
		Save the word + ID into another file.

		MUST add embedings. 





	MODEL:
		Multiple selectable models 
		Model merging (already possible with operator+=)
		Model name
		Comparing models
			compares vocabulary
			transition map size and all total transitions
			probabilities
			whatever else

			Syntax ideea:
				@model>compare{A=test_1/ B=test_2} 





	TRAINING:
		Make it possible to only train certain Models
			use a bitmask for the models to trian

			Syntax Ideea:
			*DONE*
			{
				@input>train{model = ngram} trains all available ngrams

				@input>train / @input>train {} trains everything

				@input>train{model = ngram/ size = 5} trains ngrams with size <=5
			}

				@input>train {size = 5} trains legacy + ngrams with size<=5

				@input>train{model = ngram/ size = 4,3} trains only ngrams with size 4 and 3
					!!!!! MAKE SURE TO USE EITHER " , " or " | "... or maybe " & "

				@input>train{exclude-model = [model] } to exclude whatever model

				@input>train{exclude-model = ngram/ size = 5} excludes ngram withs size 5
					!!!!! DO NOT MAKE SOMETHING LIKE model = ![model]... DO NOT.... for now.

				@input>train{model = ngram/ size= 5/ size = 4} should also work





	SAVING:
		make snapshots of current models
			Syntax ideea:
				@model>snapshot{model = ngram/ size = 5/ name= [some name] }
					this saves the ngram model with size 5

				@model>snapshot{} saves everthing 
					could... maybe also use exclude-model ?

				@model>restore{} restores everything
				@model>restore{ [model] } restores that model (or if model is ngram restore all ngrams if no size is provided)
				@model>restore{ model = ngram/ size = 3,2} should restore ngrams 2 and 3



	SCRIPTS:
		lets AMLE run simple scripts
			Example syntax (syntax can change dont take this as fix):
				@if(1+1==2)
					@out:"[some message]"

				since u have no clue on how to make ur on language's syntax to be... 
					get inspiration form c++
					u already familiar with it
					then add useful things from other languages
						like how python lets u do container[::-1] to print a reversed container? 


				@-int- a (declares an int variable?!)
					
				@-int- b = a (does... what it looks like)
					but what if u can add cursed things like
						@-int- b = ?(a%2==0 : a, 0)
							inspired from ternary operator int b = (a % 2 == 0 ? a : 0)
				
				@-string- a = "hello"
					and maybe can do some nice things like 
						a += " " + "world"
						turns into "hello world"
						
						OR THIS SINCE C++ std::toupper() and std::tolowe() are... wierd
							@a = a^::upper (results into a = "HELLO WORLD"
							and now @a^::lower results into a ="hello world"
								tho still it will do std::transform() under the hood. xDDDD



				imagine making ur own versions of std::vector<> X___X
					@container-int- = std::vector<int>
					eh?! EEEHH?!... looks good?!



			U can make it simple at first...
				then later add braces {} and whatever.
				imagine making AMLE's own programming langugage
				primitive. but still. aint nobody making an "AI" run its own language scripts like this. ... or do they!?
					hope not.

		

*/