import junit.framework.*;
import java.io.File;
import java.util.Vector;
import org.omras2.*;

public class TestQuery extends TestCase
{
	File testDBFile;
	File testFeatureFile;
	protected void setUp()
	{
		testDBFile = new File("testfiles/test.adb");
		testFeatureFile = new File("testfiles/testfeature");
		if(testDBFile.exists())
			testDBFile.delete();
	}

	public void testQuery()
	{
		// Insert the same feature twice
		AudioDB testDB = new AudioDB(testDBFile);
		assertTrue("DB created", testDB.create(1, 2, 1));
		testDB.open(AudioDB.Mode.O_RDWR);
		assertTrue("Insert feature file", testDB.insert("feat1", testFeatureFile));
		assertTrue("Insert feature file again", testDB.insert("feat2", testFeatureFile));
		testDB.close();

		testDB.open(AudioDB.Mode.O_RDONLY);
		Status status = testDB.getStatus();
		assertEquals("Two features", 2, status.getNumFiles());

		Query query = new Query();
		query.setSeqLength(1);
		query.setSeqStart(0);
		query.setIncludeKeys(new String[]{"feat1"});
		query.setExcludeKeys(new String[]{"feat2"});
		Vector<Result> results = testDB.query("feat1", query);
/*		System.out.println(results.size());
		for(Result result: results)
		{
			System.out.println(result.getKey());
			System.out.println(result.getDistance());
			System.out.println(result.getQpos());
			System.out.println(result.getIpos());
		}*/
	}
/*
	public void testAdvanced()
	{
		AudioDB testDB = new AudioDB(new File("testfiles/9.adb"));
		testDB.open(AudioDB.Mode.O_RDONLY);
		Status status = testDB.getStatus();
		System.out.println(status.getNumFiles());
		Query query = new Query();
		query.setSeqLength(16);
		query.setSeqStart(0);
		query.setExcludeKeys(new String[]{"KSA_CHARM_27", "KSA_CHARM_300"});
		Vector<Result> results = testDB.query("KSA_CHARM_336", query);
		System.out.println(results.size());
		for(Result result: results)
		{
			System.out.print(result.getKey());
			System.out.print(" "+result.getDistance());
			System.out.print(" "+result.getQpos());
			System.out.println(" "+result.getIpos());
		}

	}*/

}
